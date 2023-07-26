/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// unity
using UnityEngine;

namespace Ex {
    
    public class FOPRobotComponent : ExComponent {

        private static readonly string delayControlStr = "delay_control";
        private static readonly string forceControlStr = "force_control";
        private static readonly string testTouchStr = "test_touch";
        private static readonly string positionControlStr = "position_control";

        private static readonly string delayControl = "delayControl";
        private static readonly string forceControl = "forceControl";
        private static readonly string testTouch = "testTouch";
        private static readonly string positionControl = "positionControl";

        private static readonly string slavePositionSignal = "slave position";
        private static readonly string masterPositionSignal = "master position";
        private static readonly string slaveTouchSignal = "slave touch";
        private static readonly string masterTouchSignal = "master touch";
        private static readonly string slaveForceSignal = "slave force";
        private static readonly string masterForceSignal = "master force";

        private UdpReceiver m_receiver = null;
        private UdpSender m_sender = null;
        private int m_delay = 0;
        private float m_forceRatio = 1f;

        public Vector3 posMaster        = Vector3.zero;
        public Vector3 posSlave         = Vector3.zero;
        public bool flagTouchSlave      = false;
        public bool flagTouchMaster     = false;
        public float slaveForceValue    = 0f;
        public float masterForceValue   = 0f;

        #region ex_functions
        protected override bool initialize() {

            // signals
            add_signal(slavePositionSignal);
            add_signal(masterPositionSignal);
            add_signal(slaveTouchSignal);
            add_signal(masterTouchSignal);
            add_signal(slaveForceSignal);
            add_signal(masterForceSignal);

            // slots
            add_slot("set delay", (value) => {

                m_delay = (int)value;
                if (currentC.get<bool>(delayControlStr)) {
                    m_sender.send_message(string.Format("{0} {1}", delayControl, m_delay.ToString()));
                }

            });
            add_slot("set force ratio", (value) => {

                m_forceRatio = (float)value;
                if (currentC.get<bool>(forceControlStr)) {
                    m_sender.send_message(string.Format("{0} {1}", forceControl, m_forceRatio.ToString()));
                } else if (currentC.get<bool>(testTouchStr)) {
                    m_sender.send_message(string.Format("{0} {1}", testTouch, m_forceRatio.ToString()));
                }
            });

            var wIp = NetworkInfo.get_ip_addresses(initC.get<string>("write_address"), false);
            m_sender = new UdpSender();
            if (!m_sender.initialize(initC.get<int>("write_port"), wIp[0], false)) {
                return false;
            }

            var rIp = NetworkInfo.get_ip_addresses(initC.get<string>("read_address"), false);
            m_receiver = new UdpReceiver();
            if(!m_receiver.initialize(initC.get<int>("read_port"), rIp[0], 100)) {
                return false;
            }
            m_receiver.reset_buffer_size_to_read(1024);

            return true;
        }

        protected override void start_routine() {

            m_delay       = currentC.get<int>("delay");
            m_forceRatio  = currentC.get<float>("force_ratio");

            if (currentC.get<bool>(delayControlStr)) {
                m_sender.send_message(string.Format("{0} {1}", delayControl, m_delay.ToString()));
            } else if (currentC.get<bool>(forceControlStr)) {
                m_sender.send_message(string.Format("{0} {1}", forceControl, m_forceRatio.ToString()));
            } else if (currentC.get<bool>(testTouchStr)) {
                m_sender.send_message(string.Format("{0} {1}", testTouch, m_forceRatio.ToString()));
            } else if (currentC.get<bool>(positionControlStr)) {
                m_sender.send_message(string.Format("{0} 0", positionControl));
            }
        }

        protected override void set_update_state(bool doUpdate) {
            m_receiver.set_reading_state(doUpdate);
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            m_delay = currentC.get<int>("delay");
            m_forceRatio = currentC.get<float>("force_ratio");

            if (currentC.get<bool>(delayControlStr)) {
                m_sender.send_message(string.Format("{0} {1}", delayControl, m_delay.ToString()));
            } else if (currentC.get<bool>(forceControlStr)) {
                m_sender.send_message(string.Format("{0} {1}", forceControl, m_forceRatio.ToString()));
            } else if (currentC.get<bool>(testTouchStr)) {
                m_sender.send_message(string.Format("{0} {1}", testTouch, m_forceRatio.ToString()));
            }
        }

        protected override void update() {

            var messages = m_receiver.read_all_messages();
            if (messages != null) {
                for (int ii = 0; ii < messages.Count; ++ii) {
                    process_message(messages[ii].Item3, ii == messages.Count - 1);
                }
            }
        }


        protected override void clean() {
            m_receiver.clean();
        }

        #endregion

        #region private_functions
        private void process_message(string message, bool displayLog) {

            // "[%d ;%d ;%d ;%d ;%d ;%d ;%d ;%d; %+3.3lf; %+3.3lf]"
            message = message.Substring(1, message.Length - 2);

            var split = message.Replace(" ", "").Split(';');
            if (split.Length != 10) {
                log_error(string.Format("Invalid message {0}", message));
                return;
            }

            posMaster = new Vector3(
                Converter.to_int(split[0]),
                Converter.to_int(split[1]),
                Converter.to_int(split[2])
            );
            posSlave = new Vector3(
                Converter.to_int(split[3]),
                Converter.to_int(split[4]),
                Converter.to_int(split[5])
            );

            flagTouchSlave = Converter.to_bool(split[6]);
            flagTouchMaster = Converter.to_bool(split[7]);

            slaveForceValue = Converter.to_float(split[8]);
            masterForceValue = Converter.to_float(split[9]);

            invoke_signal(slavePositionSignal, posSlave);
            invoke_signal(masterPositionSignal, posMaster);
            invoke_signal(slaveTouchSignal, flagTouchSlave);
            invoke_signal(masterTouchSignal, flagTouchMaster);
            invoke_signal(slaveForceSignal, slaveForceValue);
            invoke_signal(masterForceSignal, masterForceValue);

            if (displayLog) {
                log_message(string.Format("[PS:{0} PM:{1} TS:{2} TM:{3}",
                    Converter.to_string(posSlave), Converter.to_string(posMaster), Converter.to_string(flagTouchSlave), Converter.to_string(flagTouchMaster)));
            }
        }

        #endregion

    }
}

