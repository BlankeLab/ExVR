/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System;
using System.Collections.Generic;
using System.Linq;

// unity
using UnityEngine;


namespace Ex{


    public class SoncebozSGProfile {
        public string[] values = null;

        public string tZero() { return values[0]; }
        public string tAcc0() { return values[1]; }
        public string tAcc1() { return values[2]; }
        public string tAcc() { return values[3]; }
        public string tDec() { return values[4]; }
        public string tDec0() { return values[5]; }
        public string tDec1() { return values[6]; }
        public string tEnd() { return values[7]; }
        public string sAcc0() { return values[8]; }
        public string sAcc1() { return values[9]; }
        public string sAcc() { return values[10]; }
        public string sDec() { return values[11]; }
        public string sDec0() { return values[12]; }
        public string sDec1() { return values[13]; }
        public string sEnd() { return values[14]; }
        public string vAcc0() { return values[15]; }
        public string vAcc1() { return values[16]; }
        public string vAcc() { return values[17]; }
        public string vDec() { return values[18]; }
        public string vDec0() { return values[19]; }
        public string vDec1() { return values[20]; }
        public string vEnd() { return values[21]; }
        public string vMax() { return values[22]; }
        public string vInit() { return values[23]; }
        public string accMax() { return values[24]; }
        public string decMax() { return values[25]; }
    }

    public class SoncebozSGComponent : ExComponent{

        private static readonly string m_feedbackS = "feedback";
        private UdpCommunication m_udp = null;
        private bool m_movementStarted = false;

        int nbFeedbacksReceived = 0;
        public TransformValue lastFeedback = new TransformValue();

        public SoncebozSGProfile lastProfile = new SoncebozSGProfile();
        public List<SoncebozSGProfile> lastProfiles = new List<SoncebozSGProfile>();
        private List<Tuple<double, double, string>> triggerEvents = null;

        #region ex_functions

        protected override bool initialize() {
            
            m_udp = new UdpCommunication(
                true,
                true,
                init_config().get<int>("writing_port"),
                initC.get<string>("writing_address"),
                init_config().get<int>("reading_port"),
                initC.get<string>("reading_address")
            );

            add_signal(m_feedbackS);
            add_signal("tEnd");
            add_signal("tAcc");
            log_message(string.Format("Sonceboz communication initialized: {0}",(m_udp.receiver_initialized() && m_udp.sender_initialized())));

            return m_udp.receiver_initialized() && m_udp.sender_initialized();
        }

        protected override void start_experiment() {
            m_udp.set_reading_mode(true);
        }

        protected override void stop_experiment() {
            m_udp.set_reading_mode(false);
        }


        protected override void start_routine() {
            m_movementStarted = false;      
        }

        protected override void pre_update() {

            if (is_updating()) {
                move_plateform();
            }

            // process messages
            var messages = m_udp.read_all_messages();            
            if (messages == null) {
                return;
            }

            // process messages
            var feedbacks = new List<Tuple<Tuple<double, double, string>, string[]>>();
            var profiles  = new List<Tuple<Tuple<double, double, string>, string[]>>();
            foreach (var message in messages) {

                var content = message.Item3;
                var split = content.Split('%');
                if (split.Length < 3) {
                    continue;
                }

                string command = split[0];
                if (command.Length == 0) {
                    continue;
                }

                string id = split[1];                    
                if (command[0] == 'F') {
                    if (id[0] == '0') {
                        feedbacks.Add(new Tuple<Tuple<double, double, string>, string[]>(message, split[2].Split(' ')));
                    }
                } else if (command[0] == 'P') {
                    if (id[0] == '0') {
                        profiles.Add(new Tuple<Tuple<double, double, string>, string[]>(message, split[2].Replace(" ", "").Split(';')));
                    }
                }
            }

            if (feedbacks.Count > 0) { 
                if (process_feedbacks(feedbacks)) {
                    if (is_updating()) {
                        invoke_signal(m_feedbackS, lastFeedback);
                    }
                }                
            }
            if(profiles.Count > 0) {
                if (process_profiles(profiles)) {
                    if (is_updating()) {                        
                        invoke_signal("tEnd", Converter.to_float(lastProfile.tEnd()));
                        invoke_signal("tAcc", Converter.to_float(lastProfile.tAcc()));
                    }
                }
            }
        }


        protected override void clean() {
            m_udp.clean();
        }

        public override string format_frame_data_for_global_logger(bool header) {

            if (header) {
                return "Sonceboz: nb_feedbacks tx_n ty_n tz_n rx_n ry_n rz_n";
            }

            return string.Format("{0} {1} {1}",
                Converter.to_int(nbFeedbacksReceived),
                Converter.to_string(lastFeedback.position, Converter.g7, " "),
                Converter.to_string(lastFeedback.rotation.eulerAngles, Converter.g7, " "));
        }

        public override List<Tuple<double, double, string>> format_trigger_data_for_global_logger() {
            var tEvents = triggerEvents;
            triggerEvents = null;
            return tEvents;
        }


        #endregion

        #region private_functions

        private void move_plateform() {

            if (!m_movementStarted) {
                if (currentC.get<bool>("apply_command_list")) {

                    var data = currentC.get_resource_text_data("command_list_file");
                    if (data != null) {
                        log_message(string.Concat("Send command to sonceboz platform: ", Converter.to_string(data.content.Length)));
                        m_udp.send_message(string.Format("$$FILE$${0}", data.content));
                    } else {
                        var message = currentC.get<string>("command_list_text");
                        if (message.Length > 0) {
                            log_message(string.Concat("Send command to sonceboz platform: ", Converter.to_string(message.Length)));
                            m_udp.send_message(string.Format("$$FILE$${0}", message));
                        } else {
                            log_warning("Message is empty, no command has been sended.");
                        }
                    }
                }
                m_movementStarted = true;
            }
        }

        private bool process_feedbacks(List<Tuple<Tuple<double, double, string>, string[]>> messages) {

            // F%0%1.58784 -9.02029 49.223 1.61245 2.41867 0
            var lastFeedbackMessage = messages[messages.Count - 1];

            var values = lastFeedbackMessage.Item2;
            if (values.Length < 6) {
                return false;
            }

            lastFeedback.position =
                new Vector3(
                    Converter.to_float(values[0]),
                    Converter.to_float(values[2]),
                    Converter.to_float(values[1])
            );
            lastFeedback.rotation = Quaternion.Euler(new Vector3(
                    Converter.to_float(values[3]),
                    Converter.to_float(values[4]),
                    Converter.to_float(values[5])
            ));
            nbFeedbacksReceived = messages.Count;
            return true;
        }

        private bool process_profiles(List<Tuple<Tuple<double, double, string>, string[]>> messages) {

            // P%0%
            // 0; 1.5; 1.50241; 3.00241; 3.00241; 4.50241; 4.50483;
            // 6.00483; 34.5; 34.6668; 207.5; 207.5; 380.333; 380.5; 415; 69;
            // 69.2221; 138.222; 138.222; 69.2221; 69; 0; 138.222; 0; 92; 92            
            string[] lastMessage = null;
            foreach(var message in messages) {

                if (message.Item2.Length <= 25) {
                    continue;
                }

                if (triggerEvents == null) {
                    triggerEvents = new List<Tuple<double, double, string>>();
                }
                triggerEvents.Add(new Tuple<double, double, string>(message.Item1.Item1, message.Item1.Item2, Text.join_with_space(message.Item2)));
                lastMessage = message.Item2;
            }

            if(lastMessage != null) {
                lastProfile = new SoncebozSGProfile();
                lastProfile.values = lastMessage;
                return true;
            }

            return false;
        }

        #endregion
    }
}
