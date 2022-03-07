
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

using MP = Biopac.API.MPDevice.MPDevImports;
using MPCODE = Biopac.API.MPDevice.MPDevImports.MPRETURNCODE;

namespace Ex {


    public class BiopacComponent : ExComponent{

        // infos
        public BiopacSettings bps = null;

        // thread
        public BiopacAcquisitionThread acquisitionThread = null;

        // signals
        private static readonly string lastValueChannelStr       = "channelX last value";
        private static readonly string lastRangeValuesChannelStr = "channelX last range values";
        private static readonly string endRoutineDataLogStr      = "end routine data log";
        private static readonly string channelsLatencyStr        = "channels latency";
        private static readonly string triggerChannelsStr        = "trigger channels";

        private static readonly List<int> samplingRateValues = new List<int>(new int[] {
            10,25,50,100,200,250,500,1000,2000,2500,5000,10000,20000,25000
        });

        private bool addHeaderLine = false;

        // debug
        private bool debugBypass = false;

        private bool connect_device() {

            int retry = 10;
            for (int ii = 0; ii < retry; ++ii) {

                log_message(string.Format("Try to connect, try n°{0} with method {1}", ii.ToString(), bps.connection.ToString()));

                // remember to change the parameters to suit your MP configuration
                // Auto connect to MP150 was introduced in BHAPI 1.1
                // passing "AUTO" or "auto" instead of the full serial number of the MP150
                // will cause BHAPI to connect to the first respoding MP150.  This is usually
                // the closest MP150 to the host computer.
                var retval = MP.connectMPDev(bps.device, bps.connection, initC.get<string>("serial"));
                if (retval == MPCODE.MPNOTINNET) {
                    log_error(string.Format("MP150 with serial number '{0}' is not in the network.", initC.get<string>("serial")));
                    continue;
                }

                if (retval == MPCODE.MPSUCCESS) {
                    log_message("Connection established.");
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

            // ex:
            // set sample rate to 5 msec per sample = 200 Hz  -> retval = setSampleRate(5.0);

            // set sampling rate (25 kHz maximum)
            double sampleRate = 1000.0 / samplingRateValues[initC.get<int>("sampling_rate_id")];
            MPCODE retval = MP.setSampleRate(sampleRate);
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Sampling error {0} with rate: {1}", BiopacSettings.code_to_string(retval), Converter.to_string(sampleRate)));
                return false;
            }
            log_message("Sample rate (Hz): " + sampleRate);

            // ex: with 16 channels
            // bool[] aCH = {
            //      true, false, false, false,
            //      false, false, false, false,
            //      false, false, false, false,
            //      false, false, false, false
            // };

            // set the acquisition channels
            retval = MP.setAcqChannels(bps.channelsState);
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
                for(int ii = 0; ii < bps.channelsId.Count; ++ii) {
                    string uid = initC.get<string>(string.Format("channel{0}_preset_uid", (bps.channelsId[ii]).ToString()));
                    if (uid.Length > 0) {
                        retval = MP.configChannelByPresetID((uint)bps.channelsId[ii], uid);
                        if (retval != MPCODE.MPSUCCESS) {
                            log_error(string.Format("Error {0} during preset file configuration for channel {1} with uid {2}", 
                                BiopacSettings.code_to_string(retval), (bps.channelsId[ii] + 1).ToString(), uid));
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

            if(acquisitionThread == null) {
                return;
            }

            // kill thread
            acquisitionThread.doLoop = false;
            Thread.Sleep(10);
            acquisitionThread.stop();

            MPCODE retval = MP.stopAcquisition();
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Cannot stop biopac acquisition, error: {0}", BiopacSettings.code_to_string(retval)));
            }

            retval = MP.disconnectMPDev();
            if (retval != MPCODE.MPSUCCESS) {
                log_error(string.Format("Cannot disconnect biopac, error: {0}", BiopacSettings.code_to_string(retval)));
            }
        }

        protected override bool initialize() {

            // initialize signals/slots
            add_slot(triggerChannelsStr, (nullArg) => { trigger_channels(); });
            add_signal(lastValueChannelStr);
            add_signal(lastRangeValuesChannelStr);
            add_signal(endRoutineDataLogStr);
            add_signal(channelsLatencyStr);

            // debug
            if (debugBypass = initC.get<bool>("debug_bypass")) {
                return true;
            }

            // init settings
            bps = new BiopacSettings();

            // # retrieve device            
            switch (initC.get<int>("device_type")) {
                case 0:
                    bps.device = MP.MPTYPE.MP36;
                    bps.nbMaxChannels = 4;
                    break;
                case 1:
                    bps.device = MP.MPTYPE.MP150;
                    bps.nbMaxChannels = 16;
                    break;
                case 2:
                    bps.device = MP.MPTYPE.MP160;
                    bps.nbMaxChannels = 16;
                    break;
            }

            // # retrieve connection
            bps.connection = (initC.get<int>("device_connection") == 0) ? MP.MPCOMTYPE.MPUSB : MP.MPCOMTYPE.MPUDP;

            // # retrieve and count enabled channels
            bps.channelsState   = new bool[bps.nbMaxChannels];
            bps.channelsName    = new List<string>();
            bps.channelsId      = new List<int>();
            for (int ii = 0; ii < bps.nbMaxChannels; ++ii) {
                bps.channelsState[ii] = initC.get<bool>(string.Format("channel{0}", ii));
                if (bps.channelsState[ii]) {
                    ++bps.enabledChannelsNb;                    
                    bps.channelsName.Add(initC.get<string>(string.Format("channel{0}_name", ii)));
                    bps.channelsId.Add(ii);
                }
            }

            // check channels
            if (bps.enabledChannelsNb == 0) {
                log_error("No channel enabled.");
                return false;
            }

            // initialize device
            if (!connect_device()) {
                return false;
            }

            if (!setup_device()) {
                return false;
            }

            if (!start_acquisition()) {
                return false;
            }

            // initialize thread
            bps.nbSamplesPerCall          = initC.get<int>("nb_samples_per_call");
            bps.samplingRate              = initC.get<int>("sampling_rate");
            bps.sizeMaxChannelSeconds     = initC.get<int>("max_nb_seconds_to_save");
            if(initC.get<int>("read_digital_mode") != 0) {
                bps.readDigitalMode = initC.get<int>("read_digital_mode") == 1 ? MP.DIGITALOPT.READ_LOW_BITS : MP.DIGITALOPT.READ_HIGH_BITS;
                bps.readDigitalEnabled = true;
            } else {
                bps.readDigitalEnabled = false;
            }
            acquisitionThread             = new BiopacAcquisitionThread();
            acquisitionThread.bps         = bps;
            acquisitionThread.processData = false;
            acquisitionThread.doLoop      = true;            
            acquisitionThread.start();
            acquisitionThread.set_priority(System.Threading.ThreadPriority.AboveNormal);

            return true;
        }

        protected override void set_update_state(bool doUpdate) {

            if (debugBypass) {
                return;
            }

            acquisitionThread.processData = doUpdate;
        }
       
        protected override void clean() {

            if (debugBypass) {
                return;
            }

            stop_acquisition();
        }

        protected override void start_experiment() {
            addHeaderLine = true;
            if (debugBypass) {
                return;
            }
            acquisitionThread.bpd.reset_settings(bps);
        }

        protected override void stop_routine() {

            if (debugBypass) {
                return;
            }

            var strData = acquisitionThread.data_to_string(addHeaderLine);
            addHeaderLine = false;

            if (strData != null) {
                invoke_signal(endRoutineDataLogStr, strData);
            }
        }

        public void trigger_channels() {

            if (debugBypass) {
                return;
            }

            var values = acquisitionThread.get_last_values();
            if (values == null) {
                return;
            }

            List<FrameTimestamp> times = values.Item1;
            List<List<double>> data    = values.Item2;

            for (int ii = 0; ii < bps.enabledChannelsNb; ++ii) {
                List<double> channelData = data[ii];
                if (channelData.Count > 0) {
                    invoke_signal(lastValueChannelStr,       new IdAny(bps.channelsId[ii] + 1, channelData[channelData.Count - 1]));
                    invoke_signal(lastRangeValuesChannelStr, new IdAny(bps.channelsId[ii] + 1, channelData));
                }
            }

            // send last latency
            if (times.Count > 0) {
                var lastFrameTimestamp = times[times.Count - 1];
                invoke_signal(channelsLatencyStr, TimeManager.ticks_to_ms(System.Diagnostics.Stopwatch.GetTimestamp() - lastFrameTimestamp.startingTick));
            }
        }
    }
}
