
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
using System.Linq;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class NetworkManager : MonoBehaviour{

        public enum Command{
            Load = 0, Play, Pause, Stop, Next, Previous, UpdateComponent, UpdateConnector, Quit,
            Action, GoToSpecificInstanceElement, PlayPause, PlayDelay, Error, Clean, UpdateDesignerDirPath
        };

        public enum ExpLauncherState : int{
            NotStarted = 0,
            Starting, // parameter: udp port/editor
            Idle,
            LoadingExperiment, // parameter: exp file / instance file
            Closing
        }

        public enum ExpState : int{
            NotLoaded = 0,
            Loaded,
            Running, // parameter: time...
            Paused   // parameter: time...
        }

        private static readonly string cancat2 = "{0}|{1}";
        private static readonly string cancat3 = "{0}|{1}|{2}";
        //private static readonly string cancat4 = "{0}|{1}|{2}|{3}";
        //private static readonly string cancat5 = "{0}|{1}|{2}|{3}|{4}";

        private ExpLauncherState m_lastExpLauncherState = ExpLauncherState.NotStarted;
        private ExpState m_lastExpState = ExpState.NotLoaded;
        private string m_lastStateMessage = "";

        public static Tuple<Command, List<string>> str_to_command(string cmdStr) {

            int idStart = cmdStr.IndexOf("[%]") + 3;
            int idEnd = cmdStr.IndexOf("[/%]");
            if (idStart != -1 && idEnd != -1) {
                var split = cmdStr.Substring(idStart, idEnd - idStart).Split(new string[] { "|" }, StringSplitOptions.None).ToList();
                if (Int32.TryParse(split[0], out int cmdInt)) {
                    split.RemoveAt(0);
                    return new Tuple<Command, List<string>>((Command)cmdInt, split);
                }
            }
            ExVR.Log().error(string.Format("Invalid command: {0}", cmdStr));
            return new Tuple<Command, List<string>>(Command.Error, new List<string>());
        }


        private Ex.GuiIpc guiIpc = null;

        public GuiIpc gui_ipc() {
            return guiIpc;
        }

        private string to_string(ExpLauncherState state) {
            return ((int)state).ToString();
        }

        private string to_string(ExpState state) {
            return ((int)state).ToString();
        }

        public void initialize() {

            // init communication
            guiIpc = GetComponent<GuiIpc>();
            guiIpc.define_udp_ports(ExVR.GuiSettings().writingPort);
            if (!guiIpc.is_initalized()) {
                ExVR.Log().error("Gui IPC not initialized, quit application.");
                return;
            }

            ExVR.Events().log.Message.AddListener((message, append) => {
                guiIpc.send_log_to_GUI(message, append);
            });
            ExVR.Events().log.Warning.AddListener((warning, append) => {
                guiIpc.send_warning_to_GUI(warning, append);
            });
            ExVR.Events().log.Error.AddListener((error, append) => {
                guiIpc.send_error_to_GUI(error, append);
            });

            guiIpc.send_exp_launcher_state_to_GUI(
                string.Format(cancat3, to_string(m_lastExpLauncherState = ExpLauncherState.Starting), guiIpc.reading_port().ToString(), ExVR.GuiSettings().editor ? "1" : "0"));
        }

        public void clean() {

            if (guiIpc != null) {

                set_experiment_state_to_GUI(ExpState.NotLoaded, "", false);

                guiIpc.send_exp_launcher_state_to_GUI(
                    to_string(m_lastExpLauncherState = ExpLauncherState.NotStarted), false);


                guiIpc.clean();
                guiIpc = null;
            }
        }



        public void set_launcher_idle_state() {
            if (m_lastExpLauncherState != ExpLauncherState.Idle) {
                guiIpc.send_exp_launcher_state_to_GUI(
                    to_string(m_lastExpLauncherState = ExpLauncherState.Idle));
            }
        }

        public void set_launcher_closing_state() {
            if (m_lastExpLauncherState != ExpLauncherState.Closing) {
                guiIpc.send_exp_launcher_state_to_GUI(
                    to_string(m_lastExpLauncherState = ExpLauncherState.Closing));
            }
        }

        public void set_launcher_loading_state(string xmlExperimentPath, string xmlInstancePath) {
            guiIpc.send_exp_launcher_state_to_GUI(string.Format(cancat3, to_string(m_lastExpLauncherState = ExpLauncherState.LoadingExperiment), xmlExperimentPath, xmlInstancePath));
        }


        public void set_experiment_state_to_GUI(ExpState state, string infos = "", bool append = false) {

            if (infos.Length == 0) {
                if (m_lastExpState != state) {
                    guiIpc.send_experiment_state_to_GUI(to_string(m_lastExpState = state), append);
                }
            } else {
                if ((m_lastExpState != state) || (m_lastStateMessage != infos)) {
                    guiIpc.send_experiment_state_to_GUI(string.Format(cancat2, to_string(m_lastExpState = state), m_lastStateMessage = infos), append);
                }
            }
        }





        public void send_command_from_exp_launcher(Command command, string infos = "") {
            switch (command) {
                case Command.Load:
                    guiIpc.debug_fake_communication(string.Format("[%]0|{0}[/%]", infos));
                    break;
                case Command.Stop:
                    guiIpc.debug_fake_communication("[%]3[/%]");
                    break;
                case Command.Play:
                    guiIpc.debug_fake_communication("[%]1[/%]");
                    break;
                case Command.Pause:
                    guiIpc.debug_fake_communication("[%]2[/%]");
                    break;                    
                case Command.PlayPause:
                    guiIpc.debug_fake_communication("[%]1[/%]");
                    guiIpc.debug_fake_communication("[%]2[/%]");
                    break;
                case Command.Quit:
                    guiIpc.debug_fake_communication("[%]8[/%]");
                    break;
                case Command.Next:
                    guiIpc.debug_fake_communication("[%]4[/%]");
                    break;
                case Command.Previous:
                    guiIpc.debug_fake_communication("[%]5[/%]");
                    break;
            }            
        }
    }
}