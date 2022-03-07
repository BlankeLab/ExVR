
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


namespace Ex {

    public class CppComponent : ExComponent {

        protected DLL.ExComponentDLL cppDll = null;

        // once per loading
        protected override bool initialize() {if (cppDll != null) { return cppDll.initialize(); } return false;}
        protected override void clean() {if (cppDll != null) {cppDll.clean();}}

        // once per experiment
        protected override void start_experiment() { if (cppDll != null) { cppDll.start_experiment(); } }
        protected override void stop_experiment() {if (cppDll != null) { cppDll.stop_experiment(); }}

        // once per routine
        protected override void set_current_config(string configName) { if (cppDll != null) { cppDll.set_current_config(configName); } }
        public override void update_from_current_config() {if (cppDll != null) { cppDll.update_from_current_config(); }}
        protected override void pre_start_routine() { if (cppDll != null) { cppDll.pre_start_routine(); } }
        protected override void start_routine() { if (cppDll != null) { cppDll.start_routine(); } }
        protected override void post_start_routine() { if (cppDll != null) { cppDll.post_start_routine(); } }
        protected override void stop_routine() { if (cppDll != null) { cppDll.stop_routine(); } }

        // several times per routine
        protected override void set_visibility(bool visible) { if (cppDll != null) { cppDll.set_visibility(visible); } }
        protected override void set_update_state(bool doUpdate) { if (cppDll != null) { cppDll.set_update_state(doUpdate); } }
        public override void play() { if (cppDll != null) { cppDll.play(); } }
        public override void pause() { if (cppDll != null) { cppDll.pause(); } }

        // every frame or more
        protected override void on_gui() { if (cppDll != null) { cppDll.on_gui(); } }
        protected override void pre_update() { if (cppDll != null) { cppDll.pre_update(); } }
        protected override void update() { if (cppDll != null) { cppDll.update(); } }
        protected override void post_update() { if (cppDll != null) { cppDll.post_update(); } }

        // from gui
        protected override void update_parameter_from_gui(string updatedArgName) { if (cppDll != null) { cppDll.update_parameter_from_gui(updatedArgName); } }
        protected override void action_from_gui(bool initConfig, string action) { if (cppDll != null) { cppDll.action_from_gui(initConfig, action); } }
    }
}