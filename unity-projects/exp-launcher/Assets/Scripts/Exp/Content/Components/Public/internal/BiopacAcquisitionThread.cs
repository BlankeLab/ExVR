
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
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;

using MP        = Biopac.API.MPDevice.MPDevImports;
using MPCODE    = Biopac.API.MPDevice.MPDevImports.MPRETURNCODE;

namespace Ex {

    //MPSUCCESS = 1,
    //MPDRVERR,
    //MPDLLBUSY,
    //MPINVPARA,
    //MPNOTCON,
    //MPREADY,
    //MPWPRETRIG,
    //MPWTRIG,
    //MPBUSY,
    //MPNOACTCH,
    //MPCOMERR,
    //MPINVTYPE,
    //MPNOTINNET,
    //MPSMPLDLERR,
    //MPMEMALLOCERR,
    //MPSOCKERR,
    //MPUNDRFLOW,
    //MPPRESETERR,
    //MPPARSERERR

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
        public int sizeMaxChannelSeconds = 0;

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

    public class BiopacData {

        // infos
        public int nbDataPointsPerFrame;
        public uint numberOfDataPoints;
        // raw data
        public double[] buffer = null;
        public bool[] digitalIO = null;
        // data
        public List<List<double>> channelsData = null;
        public List<List<double>> chanelsDataValuesSinceLastGet = null;
        public List<List<bool>> digitalInputData = null;        
        // time
        public List<long> channelsTime = null; // timestamp / data size
        public List<FrameTimestamp> timesSinceLastGet = null;
        // string
        public string headerLine;

        public BiopacData(BiopacSettings bps) {
            reset_settings(bps);
        }

        public void reset_settings(BiopacSettings bps) {

            // init header line        
            System.Text.StringBuilder titleLineB = new System.Text.StringBuilder();

            // # time column
            titleLineB.Append("[Timestamp(ms)]\t");

            // # channels data columns
            for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                titleLineB.AppendFormat("[Val-{0}]\t", bps.channelsName[ii]);
            }

            // # channels digital read columns
            if (bps.readDigitalEnabled) {
                for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                    titleLineB.AppendFormat("[Tr-{0}]\t", bps.channelsName[ii]);
                }
            }
            headerLine = titleLineB.ToString(); ;

            nbDataPointsPerFrame = bps.nbSamplesPerCall;
            numberOfDataPoints = (uint)(nbDataPointsPerFrame * bps.enabledChannelsNb);
            buffer = new double[numberOfDataPoints];
            digitalIO = new bool[bps.enabledChannelsNb];
            for (int ii = 0; ii < digitalIO.Length; ++ii) {
                digitalIO[ii] = false;
            }

            // reset data
            reset_data(bps);
        }

        public void reset_data(BiopacSettings bps) {

            int capacity1 = (int)bps.samplingRate * bps.sizeMaxChannelSeconds;
            int capacity2 = bps.sizeMaxChannelSeconds;

            channelsData = new List<List<double>>(bps.enabledChannelsNb);
            digitalInputData = new List<List<bool>>(bps.enabledChannelsNb);
            chanelsDataValuesSinceLastGet = new List<List<double>>();

            channelsTime = new List<long>(bps.sizeMaxChannelSeconds);
            timesSinceLastGet = new List<FrameTimestamp>();

            for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                channelsData.Add(new List<double>(capacity1));
                digitalInputData.Add(new List<bool>(capacity2));
                chanelsDataValuesSinceLastGet.Add(new List<double>());
            }
        }
    }

    public class BiopacAcquisitionThread : ThreadedJob {

        public BiopacSettings bps = null;
        public BiopacData bpd     = null;

        volatile public bool doLoop         = false;
        volatile public bool processData    = false;
        private ReaderWriterLock rwl        = new ReaderWriterLock();

        private System.Text.StringBuilder digitalInputF  = new System.Text.StringBuilder();
        private System.Text.StringBuilder channelsF = new System.Text.StringBuilder();

        private static readonly string tab = "\t";
        private static readonly string timestampStrAccuracy = "G8";
                

        protected override void ThreadFunction() {

            // initialize data
            bpd = new BiopacData(bps);

            ExVR.Log().message(string.Format("Number of point asked per call per chanel: {0}", bpd.nbDataPointsPerFrame.ToString()));
            ExVR.Log().message(string.Format("Number of point per call to receiveMPData: {0}", bpd.numberOfDataPoints.ToString()));

            // start reading loop
            while (doLoop) {

                FrameTimestamp times = new FrameTimestamp();

                // retrieve points
                times.startingTick          = Stopwatch.GetTimestamp();
                MPCODE retval               = MP.receiveMPData(bpd.buffer, bpd.numberOfDataPoints, out uint received);
                times.afterDataReadingTick  = Stopwatch.GetTimestamp();

                // check for error condition
                if (retval != MPCODE.MPSUCCESS ) {
                    ExVR.Log().error(string.Format("Biopack receiveMPData error code: {0}", BiopacSettings.code_to_string(retval)));
                    continue;
                }

                if (received != bpd.numberOfDataPoints) {
                    ExVR.Log().warning(string.Format("Biopack receiveMPData warning, received {0} values instead of {1}", received, bpd.numberOfDataPoints));
                }

                // retrieve values from the Digital Lines
                if (bps.readDigitalEnabled) {
                    for (int ii = 0; ii < bps.channelsId.Count; ++ii) {

                        uint channelId = (uint)bps.channelsId[ii];
                        retval = MP.getDigitalIO(channelId, out bpd.digitalIO[ii], bps.readDigitalMode);

                        if (retval != MPCODE.MPSUCCESS) {
                            ExVR.Log().error(string.Format("Biopack getDigitalIO({0}) error code {1} ", channelId, BiopacSettings.code_to_string(retval)));
                            continue;
                        }
                    }
                }

                times.afterDigitalIoReadingTick = Stopwatch.GetTimestamp();
                
                // if routine not started we don't process data
                //if (!routineStarted  || !processData) {
                if (!processData) {
                    continue;
                }

                // store values to corresponding channel array
                bpd.channelsTime.Add(times.startingTick); // keep timestamp before reading

                // received
                for (int ii = 0; ii < (int)received; ++ii) {
                    int idChannel = ii % bps.enabledChannelsNb;
                    bpd.channelsData[idChannel].Add(bpd.buffer[ii]);
                }

                // store digital line IO 
                if (bps.readDigitalEnabled) {
                    for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                        bpd.digitalInputData[ii].Add(bpd.digitalIO[ii]);
                    }
                }

                // store to last values
                try {
                    rwl.AcquireWriterLock(5);
                    try {
                        bpd.timesSinceLastGet.Add(times);

                        for (int ii = 0; ii < (int)received; ++ii) {
                            int idChannel = ii % bps.enabledChannelsNb;
                            bpd.chanelsDataValuesSinceLastGet[idChannel].Add(bpd.buffer[ii]);
                        }

                    } finally {
                        // Ensure that the lock is released.
                        rwl.ReleaseWriterLock();
                    }
                } catch (ApplicationException) {
                    ExVR.Log().error("Can't get writer lock.");
                }
            }
        }

        public Tuple<List<FrameTimestamp>, List<List<double>>> get_last_values() {
            
            List<FrameTimestamp> newTimes  = new List<FrameTimestamp>();
            List<List<double>> newData     = new List<List<double>>(bps.enabledChannelsNb);

            List<List<double>> swapData    = null;            
            List<FrameTimestamp> swapTimes = null;

            for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                newData.Add(new List<double>());
            }

            try {

                rwl.AcquireReaderLock(1);
                try {

                    // swap times
                    swapTimes = bpd.timesSinceLastGet;
                    bpd.timesSinceLastGet = newTimes;

                    // swap data
                    swapData = bpd.chanelsDataValuesSinceLastGet;
                    bpd.chanelsDataValuesSinceLastGet = newData;

                } finally {
                    // Ensure that the lock is released.
                    rwl.ReleaseReaderLock();
                }
            } catch (ApplicationException) {
                return null;
            }

            return Tuple.Create(swapTimes, swapData);
        }

        public List<string> data_to_string(bool addHeaderLine) {

            // compute capacities
            int capacity1 = (int)bps.samplingRate * bps.sizeMaxChannelSeconds;
            int capacity2 = bps.sizeMaxChannelSeconds;

            // init swaps and new data
            // # channels time
            List<long> swapChannelsTime = null;
            List<long> newChannelsTime = new List<long>(bps.sizeMaxChannelSeconds);
            // # channels data
            List<List<double>> swapChannelsData = null;
            List<List<double>> newChannelsData = new List<List<double>>(bps.enabledChannelsNb);
            // # digital input data
            List<List<bool>> swapDigitalInputData = null;
            List<List<bool>> newDigitalInputData = bps.readDigitalEnabled ? new List<List<bool>>(bps.enabledChannelsNb) : null;

            // populate
            for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                newChannelsData.Add(new List<double>(capacity1));
                if (newDigitalInputData != null) {
                    newDigitalInputData.Add(new List<bool>(capacity2));
                }
            }

            // do the swap while keeping the lock
            try {
                rwl.AcquireWriterLock(5);
                try {

                    if (bpd.channelsTime.Count == 0) {
                        rwl.ReleaseWriterLock();
                        return null;
                    }

                    // swap data
                    // # channels time
                    swapChannelsTime = bpd.channelsTime;
                    bpd.channelsTime = newChannelsTime;                    
                    // # channels data
                    swapChannelsData = bpd.channelsData;
                    bpd.channelsData = newChannelsData;                    
                    // # digital input data
                    if (bps.readDigitalEnabled) {
                        swapDigitalInputData = bpd.digitalInputData;
                        bpd.digitalInputData = newDigitalInputData;
                        
                    }

                } finally {
                    rwl.ReleaseWriterLock();
                }
            } catch (ApplicationException) {
                return null;
            }

            newChannelsTime = null;
            newChannelsData = null;
            newDigitalInputData = null;


            var numValuesPerTime = swapChannelsData[0].Count / swapChannelsTime.Count;
            int currentValueId = 0;


            List<string> dataStr = new List<string>(swapChannelsTime.Count * numValuesPerTime + 1);
            if (addHeaderLine) {
                dataStr.Add(bpd.headerLine);
            }
         
            for (int ii = 0; ii < swapChannelsTime.Count; ++ii) {

                // time per frame
                var time = ExVR.Time().ms_since_start_experiment(swapChannelsTime[ii]);
                if(time < 0) {
                    time = 0;
                }
                var timeStampStr = string.Concat(Converter.to_string(time, timestampStrAccuracy), tab);

                // digital input per channel
                if (bps.readDigitalEnabled) {
                    digitalInputF.Clear();
                    for (int jj = 0; jj < bps.enabledChannelsNb; ++jj) {
                        digitalInputF.Append(string.Concat(Converter.to_string(swapDigitalInputData[jj][ii], false), tab));
                    }
                }

                // values per channel
                for (int jj = 0; jj < numValuesPerTime; ++jj) {

                    channelsF.Clear();
                    for (int channel = 0; channel < bps.enabledChannelsNb; ++channel) {
                        if (currentValueId < swapChannelsData[channel].Count) { // in case we receive less value
                            channelsF.Append(string.Concat(Converter.to_string(swapChannelsData[channel][currentValueId]), tab));
                        } else {
                            break;
                        }
                    }
                    ++currentValueId;

                    if (bps.readDigitalEnabled) {
                        dataStr.Add(string.Concat(timeStampStr, channelsF.ToString(), digitalInputF.ToString()));
                    } else {
                        dataStr.Add(string.Concat(timeStampStr, channelsF.ToString()));
                    }
                }
            }

            ExVR.Log().message(string.Format("Number of entries: {0}", dataStr.Count.ToString()));
            return dataStr;
        }
    }
}