
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
using System.Threading;
using System.Collections.Generic;

// unity
using UnityEngine.Profiling;

using MP     = Biopac.API.MPDevice.MPDevImports;
using MPCODE = Biopac.API.MPDevice.MPDevImports.MPRETURNCODE;

namespace Ex {

    public class BiopacComponent : ExComponent{

        // infos
        public BiopacSettings bSettings = null;

        // thread
        public BiopacAcquisitionThread acquisitionThread = null;
        public BiopacProcessingThread processThread = null;

        // signals
        private static readonly string lastValueChannelStr       = "channelX last value";
        private static readonly string lastRangeValuesChannelStr = "channelX last range values";
        //private static readonly string endRoutineDataLogStr      = "end routine data log";
        private static readonly string callDurationAPIStr        = "call duration API";
        private static readonly string readDataStr               = "read data";
        private static readonly List<int> samplingRateValues = new List<int>(new int[] {
            10,25,50,100,200,250,500,1000,2000,2500,5000,10000,20000,25000
        });

        private List<System.Tuple<double, List<double>>> m_debugData = null;
        private int m_currentDebugId = 0;
        private bool m_debugBypass = false;
       

        #region ex_functions

        protected override bool initialize() {

            // initialize signals/slots
            add_slot(readDataStr, (nullArg) => {  /** TODO: to remove from signals list */});
            add_signal(lastValueChannelStr);
            add_signal(lastRangeValuesChannelStr);
            add_signal(callDurationAPIStr);



            // init settings
            bSettings = new BiopacSettings();

            // # retrieve device            
            switch (initC.get<int>("device_type")) {
                case 0:
                    bSettings.device = MP.MPTYPE.MP36;
                    bSettings.nbMaxChannels = 4;
                    break;
                case 1:
                    bSettings.device = MP.MPTYPE.MP150;
                    bSettings.nbMaxChannels = 16;
                    break;
                case 2:
                    bSettings.device = MP.MPTYPE.MP160;
                    bSettings.nbMaxChannels = 16;
                    break;
            }

            // # retrieve connection
            bSettings.connection = (initC.get<int>("device_connection") == 0) ? MP.MPCOMTYPE.MPUSB : MP.MPCOMTYPE.MPUDP;

            // # retrieve and count enabled channels
            bSettings.channelsState   = new bool[bSettings.nbMaxChannels];
            bSettings.channelsName    = new List<string>();
            bSettings.channelsId      = new List<int>();
            for (int ii = 0; ii < bSettings.nbMaxChannels; ++ii) {
                bSettings.channelsState[ii] = initC.get<bool>(string.Format("channel{0}", ii));
                if (bSettings.channelsState[ii]) {
                    ++bSettings.enabledChannelsNb;                    
                    bSettings.channelsName.Add(initC.get<string>(string.Format("channel{0}_name", ii)));
                    bSettings.channelsId.Add(ii);
                }
            }

            // check channels
            if (bSettings.enabledChannelsNb == 0) {
                log_error("No channel enabled.");
                return false;
            }

            bSettings.nbSamplesPerCall      = initC.get<int>("nb_samples_per_call");
            bSettings.samplingRate          = samplingRateValues[initC.get<int>("sampling_rate_id")];
            bSettings.numberOfDataPoints    = (uint)(bSettings.nbSamplesPerCall * bSettings.enabledChannelsNb);

            var digitalMode = initC.get<int>("read_digital_mode");
            if (digitalMode != 0) {
                bSettings.readDigitalMode = digitalMode == 1 ? MP.DIGITALOPT.READ_LOW_BITS : MP.DIGITALOPT.READ_HIGH_BITS;
                bSettings.readDigitalEnabled = true;
            } else {
                bSettings.readDigitalEnabled = false;
            }

            // init header line        
            System.Text.StringBuilder titleLineB = new System.Text.StringBuilder();

            // # id and times           
            titleLineB.Append("[Element order]\t");
            titleLineB.Append("[Routine-condition]\t");
            titleLineB.Append("[Total Id sample]\t");
            titleLineB.Append("[Id packet]\t");
            titleLineB.Append("[Id sample]\t");
            titleLineB.Append("[Sample exp time (ms)]\t");
            titleLineB.Append("[Sample time relative to t0(ms)]\t");
            titleLineB.Append("[Before acquisition exp time (ms)]\t");
            titleLineB.Append("[Acquisition duration (ms)]\t");

            // # channels data columns
            for (int ii = 0; ii < bSettings.enabledChannelsNb; ++ii) {
                titleLineB.AppendFormat("[Val-{0}]\t", bSettings.channelsName[ii]);
            }

            // # channels digital read columns
            if (bSettings.readDigitalEnabled) {
                for (int ii = 0; ii < bSettings.enabledChannelsNb; ++ii) {
                    titleLineB.AppendFormat("[Tr-{0}]\t", bSettings.channelsName[ii]);
                }
            }

            // debug
            if (m_debugBypass = initC.get<bool>("debug_bypass")) {
                read_debug_file();
                return true;
            }

            // initialize biopac device
            if (!connect_device()) {
                return false;
            }
            if (!setup_device()) {
                return false;
            }
            if (!start_acquisition()) {
                return false;
            }

            // init threads
            // # acquisition
            acquisitionThread                   = new BiopacAcquisitionThread();
            acquisitionThread.bSettings         = bSettings;
            acquisitionThread.start(ThreadPriority.AboveNormal);

            // # process
            processThread                       = new BiopacProcessingThread();            
            processThread.headerLine            = titleLineB.ToString();
            processThread.bSettings             = bSettings;
            processThread.acquisitionThread     = acquisitionThread;
            processThread.logger                = initC.get_component<LoggerComponent>("logger");

            return true;
        }
       
        protected override void clean() {

            if (m_debugBypass) {
                return;
            }

            // join threads
            //acquisitionThread.stop_loop();
            acquisitionThread.stop_loop();
            if (!acquisitionThread.join(500)) {
                log_error(string.Format("Stop acquisition thread timeout."));
            }
            acquisitionThread = null;

            if (!processThread.join(500)) {
                log_error(string.Format("Stop process thread timeout."));
            }
            processThread = null;

            stop_acquisition();
        }

        protected override void post_start_experiment() {

            m_currentDebugId   = 0;            
            if (m_debugBypass) {
                return;
            }

            acquisitionThread.record = true;

            processThread.addHeaderLine = true;                        
            processThread.start(ThreadPriority.Normal);                        
        }

        protected override void pre_stop_experiment() {

            if (m_debugBypass) {
                return;
            }

            acquisitionThread.stop_recording();
            processThread.stop_loop();
        }


        protected override void update() {

            if (m_debugBypass) {
                trigger_debug();
                return;
            }            

            BData lastData;
            List<BData> data = null; 
            while(processThread.dataList.TryDequeue(out lastData)) {
                if(data == null) {
                    data = new List<BData>();
                }
                data.Add(lastData);
            }

            if (data != null) {
                Profiler.BeginSample("[biopac] main trigger");
                trigger_channels(data);
                Profiler.EndSample();
            }
        }


        #endregion

        #region private_functions
        private bool connect_device() {

            int retry = 3;
            for (int ii = 0; ii < retry; ++ii) {

                log_message( message : string.Format("Try to connect, try n°{0} with method {1}", (ii+1).ToString(), bSettings.connection.ToString()),  append : false);

                // remember to change the parameters to suit your MP configuration
                // Auto connect to MP150 was introduced in BHAPI 1.1
                // passing "AUTO" or "auto" instead of the full serial number of the MP150
                // will cause BHAPI to connect to the first respoding MP150.  This is usually
                // the closest MP150 to the host computer.
                var retval = MP.connectMPDev(bSettings.device, bSettings.connection, initC.get<string>("serial"));
                if (retval == MPCODE.MPNOTINNET) {
                    log_error(string.Format("MP150 with serial number '{0}' is not in the network.", initC.get<string>("serial")));
                    continue;
                }

                if (retval == MPCODE.MPSUCCESS) {
                    log_message(message : "Connection established.", append : false);
                    return true;
                } else {
                    log_error(string.Format("connectMPDev error: {0}", BiopacSettings.code_to_string(retval)));
                }
            }

            MP.disconnectMPDev();
            log_error("Cannot connect to the device. No data will be received.", false);

            return false;
        }

        private bool setup_device() {

            // ex: set sample rate to 5 msec per sample = 200 Hz  -> retval = setSampleRate(5.0);
            double sampleRate = 1000.0 / samplingRateValues[initC.get<int>("sampling_rate_id")];
            MPCODE retval = MP.setSampleRate(sampleRate);
            log_message(message : string.Format("Frequency {0} {1} {2}", initC.get<int>("sampling_rate_id"), samplingRateValues[initC.get<int>("sampling_rate_id")], sampleRate), append : false);
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Sampling error {0} with rate: {1}", BiopacSettings.code_to_string(retval), Converter.to_string(sampleRate)));
                return false;
            }

            // set the acquisition channels
            retval = MP.setAcqChannels(bSettings.channelsState);
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Channels acquisition error {0}, disconnecting device", BiopacSettings.code_to_string(retval)));
                return false;
            }

            // read xml preset file
            var presetTxTData = initC.get_resource_text_data("preset_xml_file");
            if (presetTxTData != null) {
                retval = MP.loadXMLPresetFile(presetTxTData.path);
                if (retval != MPCODE.MPSUCCESS) {
                    log_error(string.Format("Error during xml preset loading {0}", BiopacSettings.code_to_string(retval)));
                    return false;
                }

                // set preset id for each channel
                for (int ii = 0; ii < bSettings.channelsId.Count; ++ii) {
                    string uid = initC.get<string>(string.Format("channel{0}_preset_uid", (bSettings.channelsId[ii]).ToString()));
                    if (uid.Length > 0) {
                        retval = MP.configChannelByPresetID((uint)bSettings.channelsId[ii], uid);
                        if (retval != MPCODE.MPSUCCESS) {
                            log_error(string.Format("Error {0} during preset file configuration for channel {1} with uid {2}",
                                BiopacSettings.code_to_string(retval), (bSettings.channelsId[ii] + 1).ToString(), uid));
                            return false;
                        }
                    }
                }
            }

            return true;
        }

        private bool start_acquisition() {

            // start acquisition daemon
            MPCODE retval = MP.startMPAcqDaemon();
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Biopac acquisition failed: cannot start acquisition daemon, error: {0}", BiopacSettings.code_to_string(retval)));

                retval = MP.stopAcquisition();
                if (retval != MPCODE.MPSUCCESS) {
                    log_error(string.Format("Cannot stop biopac acquisition, error: {0}", BiopacSettings.code_to_string(retval)));
                }

                return false;
            }

            // start biopac acquisition
            retval = MP.startAcquisition();
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Biopac acquisition failed: cannot start, error: {0}", BiopacSettings.code_to_string(retval)));

                retval = MP.stopAcquisition();
                if (retval != MPCODE.MPSUCCESS) {
                    log_error(string.Format("Cannot stop biopac acquisition, error: {0}", BiopacSettings.code_to_string(retval)));
                }

                return false;
            }

            return true;
        }

        private void stop_acquisition() {


            MPCODE retval = MP.stopAcquisition();
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Cannot stop biopac acquisition, error: {0}", BiopacSettings.code_to_string(retval)));
            }

            retval = MP.disconnectMPDev();
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Cannot disconnect biopac, error: {0}", BiopacSettings.code_to_string(retval)));
            }
        }

        void read_debug_file() {

            if (initC.get_resource_alias("debug_log_file").Length > 0) {

                var content = Text.split_lines(initC.get_resource_text_data("debug_log_file").content);
                log_message("content: " + content.Length);
                m_debugData = new List<System.Tuple<double, List<double>>>(content.Length);
                int nbCols = 0;
                foreach (var line in content) {
                    if (line.StartsWith("[Routine")) {
                        continue;
                    }
                    if (line.StartsWith("[Timestamp(ms)]	")) {
                        nbCols = Text.split_tabs(line).Length;
                    }
                    if (nbCols == 0) {
                        continue;
                    }
                    var split = Text.split_tabs(line);
                    if (split.Length != nbCols) {
                        continue;
                    }
                    var lineValues = new System.Tuple<double, List<double>>(Converter.to_double(split[0]), new List<double>(nbCols));
                    for (int ii = 1; ii < nbCols; ++ii) {
                        lineValues.Item2.Add(Converter.to_double(split[ii]));
                    }
                    m_debugData.Add(lineValues);
                }
            } 
           
        }

        private void trigger_channels(List<BData> lastDataRange) {

            bool sendLastValue = is_signal_connected(lastValueChannelStr);
            bool sendRange     = is_signal_connected(lastRangeValuesChannelStr);

            if(sendRange || sendLastValue) {

                List<List<double>> rangeValuesPerChannel = null;
                List<double> lastValuePerChannel = null;
                if (sendRange) {
                    rangeValuesPerChannel = new List<List<double>>(bSettings.enabledChannelsNb);
                    for (int idChannel = 0; idChannel < bSettings.enabledChannelsNb; ++idChannel) {
                        rangeValuesPerChannel.Add(new List<double>(bSettings.nbSamplesPerCall));
                    }
                }
                if (sendLastValue) {
                    lastValuePerChannel = new List<double>(bSettings.enabledChannelsNb);
                }


                for (int idRange = 0; idRange < lastDataRange.Count; ++idRange) {

                    var currentD = lastDataRange[idRange];

                    int idData = 0;
                    for (int idSample = 0; idSample < bSettings.nbSamplesPerCall; ++idSample) {
                        for (int idChannel = 0; idChannel < bSettings.enabledChannelsNb; ++idChannel) {

                            if (sendRange) {
                                rangeValuesPerChannel[idChannel].Add(currentD.channelsData[idData]);
                            }

                            if (sendLastValue) {
                                if (idSample == bSettings.nbSamplesPerCall - 1) {
                                    lastValuePerChannel.Add(currentD.channelsData[idData]);
                                }
                            }
                            ++idData;
                        }
                    }
                }


                if (sendRange) {
                    for (int idChannel = 0; idChannel < bSettings.enabledChannelsNb; ++idChannel) {
                        List<double> channelData = rangeValuesPerChannel[idChannel];
                        if (channelData.Count > 0) {
                            invoke_signal(lastRangeValuesChannelStr,
                                new IdAny(
                                    bSettings.channelsId[idChannel] + 1,
                                    channelData
                                )
                            );
                        }
                    }
                }

                if (sendLastValue) {
                    for (int idChannel = 0; idChannel < bSettings.enabledChannelsNb; ++idChannel) {
                        invoke_signal(lastValueChannelStr,
                            new IdAny(
                                bSettings.channelsId[idChannel] + 1,
                                lastValuePerChannel[idChannel]
                            )
                        );
                    }
                }
            }

            for(int ii = 0; ii < lastDataRange.Count; ++ii) {
                // send last delay
                invoke_signal(callDurationAPIStr, lastDataRange[ii].afterExpTime - lastDataRange[ii].beforeExpTime);
            }
        }

        private void trigger_debug() {

            if (m_debugData != null) {
                var currTime = ExVR.Time().ellapsed_exp_ms();
                for (int ii = m_currentDebugId; ii < m_debugData.Count; ++ii) {
                    if (m_currentDebugId >= m_debugData.Count) {
                        return;
                    }
                    if (m_debugData[m_currentDebugId].Item1 < currTime) {
                        ++m_currentDebugId;
                    } else {
                        break;
                    }
                }

                for (int ii = 0; ii < m_debugData[m_currentDebugId].Item2.Count; ++ii) {
                    invoke_signal(lastValueChannelStr, new IdAny(ii + 1, m_debugData[m_currentDebugId].Item2[ii]));
                }
            }
        }

        #endregion

    }
}
