
/***********************************************************************************
** exvr-exp                                                                       **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

// system
using System;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;
using System.Collections.Concurrent;

// unity
using UnityEngine.Profiling;

using MP        = Biopac.API.MPDevice.MPDevImports;
using MPCODE    = Biopac.API.MPDevice.MPDevImports.MPRETURNCODE;

namespace Ex {

    public struct FrameTimestamp {
        public long startingTick;
        public long afterDataReadingTick;
        public long afterDigitalIoReadingTick;
    }

    public class BiopacSettings {

        public MP.DIGITALOPT readDigitalMode;
        public bool readDigitalEnabled = false;
        public MP.MPTYPE device;
        public MP.MPCOMTYPE connection;
        public int enabledChannelsNb = 0;
        public int nbMaxChannels;
        public bool[] channelsState      = null; // size: nbMaxChannels
        public List<string> channelsName = null; // size: enabledChannelsNb
        public List<int> channelsId      = null; // size: enabledChannelsNb

        public int nbSamplesPerCall = 10;
        public double samplingRate = 0.0;   
        public uint numberOfDataPoints = 0;

        public static string code_to_string(MPCODE code) {
            switch (code) {
                case MPCODE.MPSUCCESS:
                    return " [successful execution] ";
                case MPCODE.MPDRVERR:
                    return " [error communicating with the device drivers] ";
                case MPCODE.MPDLLBUSY:
                    return " [a process is attached to the DLL, only one process may use the DLL] ";
                case MPCODE.MPINVPARA:
                    return " [invalid parameter(s)] ";
                case MPCODE.MPNOTCON:
                    return " [MP device is not connected] ";
                case MPCODE.MPREADY:
                    return " [MP device is ready] ";
                case MPCODE.MPWPRETRIG:
                    return " [MP device is waiting for pre-trigger (pre-triggering is not implemented)] ";
                case MPCODE.MPWTRIG:
                    return " [MP device is waiting for trigger] ";
                case MPCODE.MPBUSY:
                    return " [MP device is busy] ";
                case MPCODE.MPNOACTCH:
                    return " [there are no active channels, in order to acquire data at least one analog channel must be active] ";
                case MPCODE.MPCOMERR:
                    return " [generic communication error] ";
                case MPCODE.MPINVTYPE:
                    return " [the function is incompatible with the selected MP device or communication method] ";
                case MPCODE.MPNOTINNET:
                    return " [the specified MP150 is not in the network] ";
                case MPCODE.MPSMPLDLERR:
                    return " [MP device overwrote samples that had not been transferred from the device (buffer overflow)] ";
                case MPCODE.MPMEMALLOCERR:
                    return " [error allocating memory] ";
                case MPCODE.MPSOCKERR:
                    return " [internal socket error] ";
                case MPCODE.MPUNDRFLOW:
                    return " [MP device returned a data pointer that is less than the last data pointer] ";
                case MPCODE.MPPRESETERR:
                    return " [error with the specified preset file] ";
                case MPCODE.MPPARSERERR:
                    return " [preset file parsing error, the XML file must be valid according to the schema] ";
                default:
                    return "[unknow error]";
            }
        }
    }

    public class BData {

        public int id                   = 0;
        public int currentElementOrder  = 0;
        public double beforeExpTime     = 0.0;
        public double afterExpTime      = 0.0;
        public double[] channelsData    = null;
        public bool[] digitalInputsData = null;
        public double[] timesData       = null;
        private static int counter      = 0;

        public BData(BiopacSettings bSettings) {
            channelsData = new double[bSettings.numberOfDataPoints];
            if (bSettings.readDigitalEnabled) {
                digitalInputsData = new bool[bSettings.enabledChannelsNb];
            }
            id = counter++;
        }
        public static void reset_counter() {
            counter = 0;
        }
    }

    public class BiopacAcquisitionThread : ThreadedJob {
        
        public BiopacSettings bSettings    = null;
        public ConcurrentQueue<BData> data = null;

        volatile public bool record = false;
        volatile private bool m_doLoop = false;
        volatile private bool m_isAcquiring = false;

        private CustomSampler retrievePointsSampler = CustomSampler.Create("[biopac] acquisition retrieve_points");        
        private CustomSampler insertDataSampler     = CustomSampler.Create("[biopac] acquisition insert_data");
        private int m_errorsCount = 0;
        public void stop_recording() {
            record = false;
            while (m_isAcquiring) {
                Thread.Sleep(1);
            }
        }

        public void stop_loop() {
            record   = false;
            m_doLoop = false;
            while (!isDone) {
                Thread.Sleep(1);
            }
        }

        protected override void thread_function() {

            Thread.CurrentThread.Name = "BiopacAcquisitionThread";
            Profiler.BeginThreadProfiling("BiopacAcquisitionThread", "BiopacAcquisitionThread 1");

            var waitBuffer = new double[bSettings.enabledChannelsNb];

            var timeM     = ExVR.Time();
            var scheduler = ExVR.Scheduler();
            m_doLoop      = true;
            m_isAcquiring = false;
            data          = new ConcurrentQueue<BData>();
            m_errorsCount = 0;
            BData.reset_counter();

            // start reading loop
            while (m_doLoop) {

                while (!record && m_doLoop) {
                    MP.receiveMPData(waitBuffer, (uint)bSettings.enabledChannelsNb, out uint notUsed);
                    continue;
                }

                m_isAcquiring = true;

                // retrieve points
                retrievePointsSampler.Begin();
                BData d = new BData(bSettings);
                d.currentElementOrder = scheduler.current_element_order();
                d.beforeExpTime       = timeM.ellapsed_exp_ms();
                
                MPCODE retval = MP.receiveMPData(d.channelsData, bSettings.numberOfDataPoints, out uint received);                    
                d.afterExpTime = timeM.ellapsed_exp_ms();

                // check for error condition
                if (retval != MPCODE.MPSUCCESS) {
                    if (m_errorsCount < 30) {
                        ExVR.Log().error(string.Format("Biopack receiveMPData error code: {0}", BiopacSettings.code_to_string(retval)));                        
                    }else if(m_errorsCount == 30) {
                        ExVR.Log().error("Biopac receiveMPData, too many errors. Stop displaying logs.");
                    }
                    ++m_errorsCount;
                    System.Threading.Thread.Sleep(1);
                    m_isAcquiring = false;
                    continue;
                }

                // check number of points read
                if (received != bSettings.numberOfDataPoints) {
                    if (m_errorsCount < 30) {
                        ExVR.Log().error(string.Format("Biopack receiveMPData warning, received {0} values instead of {1}", received, bSettings.numberOfDataPoints));
                    } else if (m_errorsCount == 30) {
                        ExVR.Log().error("Biopac receiveMPData, too many errors. Stop displaying logs.");
                    }
                    ++m_errorsCount;
                    System.Threading.Thread.Sleep(1);
                    m_isAcquiring = false;
                    continue;
                }

                // retrieve values from the Digital Lines
                if (bSettings.readDigitalEnabled) {
                    for (int ii = 0; ii < bSettings.channelsId.Count; ++ii) {
                        retval = MP.getDigitalIO((uint)bSettings.channelsId[ii], out d.digitalInputsData[ii], bSettings.readDigitalMode);
                        if (retval != MPCODE.MPSUCCESS) {
                            if (m_errorsCount < 30) {
                                ExVR.Log().error(string.Format("Biopack getDigitalIO({0}) error code {1} ",
                                    bSettings.channelsId[ii],
                                    BiopacSettings.code_to_string(retval))
                                );
                                System.Threading.Thread.Sleep(1);
                            } else if (m_errorsCount == 30) {
                                ExVR.Log().error("Biopac getDigitalIO, too many errors. Stop displaying logs.");
                            }
                            ++m_errorsCount;
                            m_isAcquiring = false;
                            continue;
                        }
                    }
                }
                retrievePointsSampler.End();

                insertDataSampler.Begin();
                data.Enqueue(d);
                insertDataSampler.End();

                m_isAcquiring = false;
            }

            Profiler.EndThreadProfiling();
        }    
    }

    public class BiopacProcessingThread : ThreadedJob {

        volatile public bool addHeaderLine = false;
        public string headerLine;
        public BiopacSettings bSettings = null;
        public BiopacAcquisitionThread acquisitionThread = null;
        public LoggerComponent logger = null;
        public ConcurrentQueue<BData> dataList = null;

        volatile private bool m_doLoop = false;
        private double m_firstExpTime = 0.0;
        private int m_prevOrder = -1;        

        private StringBuilder m_digitalInputStrF = new StringBuilder();
        private StringBuilder m_dataStrF = new StringBuilder(1000);        

        private CustomSampler m_fullSampler = CustomSampler.Create("[biopac] processing full");
        private CustomSampler m_appendDigitalSampler = CustomSampler.Create("[biopac] processing append_digital");
        private CustomSampler m_appendChannelsSampler = CustomSampler.Create("[biopac] processing append_channels");

        private static readonly string m_timestampStrAccuracy = "G8";

        public void stop_loop() {
            m_doLoop = false;
            while (!isDone) {
                Thread.Sleep(1);
            }
        }

        protected override void thread_function() {

            Thread.CurrentThread.Name = "BiopacProcessingThread";
            Profiler.BeginThreadProfiling("BiopacProcessingThread", "BiopacProcessingThread 1");

            m_doLoop     = true;
            dataList = new ConcurrentQueue<BData>();

            BData lastData;
            while (m_doLoop) {
                if (acquisitionThread.data != null) {
                    while (acquisitionThread.data.TryDequeue(out lastData)) {

                        dataList.Enqueue(lastData);
                        if (logger != null) {
                            logger.write_lines(data_to_string(lastData));
                        }

                        if (dataList.Count > 1000) {
                            dataList.TryDequeue(out lastData);
                        }
                    }
                }
                Thread.Sleep(1);
            }

            if(logger != null) {
                while (logger.is_writing()) {
                    Thread.Sleep(1);
                }
            }

            Profiler.EndThreadProfiling();
        }

        private List<string> data_to_string(BData data) {

            int nbValuesPerChannel = data.channelsData.Length / bSettings.enabledChannelsNb;

            // init lines array
            int nbLines = nbValuesPerChannel + (addHeaderLine ? 1 : 0);
            List<string> dataStr = new List<string>(nbLines);
            for (int ii = 0; ii < nbLines; ++ii) {
                dataStr.Add(null);
            }

            int idLines = 0;
            if (addHeaderLine) {
                dataStr[idLines++] = headerLine;
                addHeaderLine = false;
            }
   
            m_fullSampler.Begin();

            m_digitalInputStrF.Clear();
            m_dataStrF.Clear();

            m_appendDigitalSampler.Begin();
            string digitalInputStr = "";
            var digital = data.digitalInputsData;
            if (bSettings.readDigitalEnabled) {

                m_digitalInputStrF.Clear();
                for (int idC = 0; idC < bSettings.enabledChannelsNb; ++idC) {
                    if (idC < bSettings.enabledChannelsNb - 1) {
                        m_digitalInputStrF.Append(digital[idC] ? '1' : '0').Append('\t');
                    } else {
                        m_digitalInputStrF.Append(digital[idC] ? '1' : '0');
                    }
                }
                digitalInputStr = m_digitalInputStrF.ToString();
            }
            m_appendDigitalSampler.End();

            m_appendChannelsSampler.Begin();

            int idLinesMinusHeader = 0;
            int idData = 0;
            double rate = 1000.0 / bSettings.samplingRate;
            var idTotal = (data.id * bSettings.nbSamplesPerCall);

            var beforeExpTimeStr       = data.beforeExpTime.ToString(m_timestampStrAccuracy);
            var durationAcquisitionStr = (data.afterExpTime - data.beforeExpTime).ToString(m_timestampStrAccuracy);

            if(data.id == 0) {
                m_firstExpTime = data.beforeExpTime;
                m_prevOrder = -1;
            }

            for (int idSample = 0; idSample < bSettings.nbSamplesPerCall; ++idSample, ++idTotal) {
                              
                m_dataStrF.Clear();

                // order
                m_dataStrF.Append(data.currentElementOrder.ToString()).Append('\t');
                // routine-condition
                if (m_prevOrder != data.currentElementOrder) {
                    m_dataStrF.Append(((RoutineInfo)ExVR.Scheduler().instance.element_order(data.currentElementOrder)).full_name()).Append('\t');
                    m_prevOrder = data.currentElementOrder;
                } else {
                    m_dataStrF.Append('-').Append('\t');
                }
                // total id sample
                m_dataStrF.Append(idTotal).Append('\t');
                // id packet
                m_dataStrF.Append(data.id).Append('\t');
                // id sample
                m_dataStrF.Append(idSample).Append('\t');
                // sample exp time
                var time = (idTotal * rate);
                m_dataStrF.Append((time + m_firstExpTime).ToString(m_timestampStrAccuracy)).Append('\t');
                // sample time
                m_dataStrF.Append(time.ToString(m_timestampStrAccuracy)).Append('\t');
                // before acquisition timestamp
                m_dataStrF.Append(beforeExpTimeStr).Append('\t');
                // duration acquisition
                m_dataStrF.Append(durationAcquisitionStr).Append('\t');
                
                idLinesMinusHeader++;

                // add values
                for (int idChannel = 0; idChannel < bSettings.enabledChannelsNb; ++idChannel) {
                    m_dataStrF.Append(data.channelsData[idData++].ToString());
                    if (bSettings.readDigitalEnabled || (idChannel < bSettings.enabledChannelsNb - 1)) {
                        m_dataStrF.Append('\t');
                    }
                }

                // add digital
                if (bSettings.readDigitalEnabled) {
                    m_dataStrF.Append(digitalInputStr);
                }

                // add full line
                dataStr[idLines++] = m_dataStrF.ToString();
            }
            m_appendChannelsSampler.End();           

            m_fullSampler.End();
            return dataStr;
        }
    }
}