
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

    public class Action{

        private TimeLine m_timeline = null;
        private ComponentConfig m_config = null;
        private ExComponent m_component = null;

        public void set_config(ComponentConfig config) {
            m_config = config;
        }

        public ComponentConfig config() {
            return m_config;
        }

        public TimeLine timeline() {
            return m_timeline;
        }

        public ExComponent component() {
            return m_component;
        }
        public void force_config(ComponentConfig config) {
            m_config = config;
        }

        public Action(XML.Action actionXml) {

            // init timeline
            m_timeline = new TimeLine(actionXml.Timelines[0], actionXml.Timelines[1]);

            // retrieve component
            m_component = ExVR.Components().get_from_key(actionXml.KeyComponent);

            if (!m_component) {
                ExVR.Log().error(string.Format("Cannot init action, component with key {0} not found. ", actionXml.KeyComponent));
                return;
            }

            // retrieve config
            foreach (ComponentConfig config in m_component.configs) {
                if (config.key == actionXml.KeyConfig) {
                    m_config = config;
                    break;
                }
            }

            if (m_config == null) {
                ExVR.Log().error(string.Format("Cannot init action, config with key {0} in component {1} not found. ", actionXml.KeyConfig, m_component.name));
                return;
            }
        }

        public void set_current_config(Condition condition) {

            if (m_component.is_function_defined(ExComponent.Function.set_current_config)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.set_current_config, true, true);
            }

            m_component.base_set_current_config(condition, config(), timeline());

            if (m_component.is_function_defined(ExComponent.Function.set_current_config)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.set_current_config, false, true);
            }
        }

        public void update_from_current_config() {

            if (m_component.is_function_defined(ExComponent.Function.update_from_current_config)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.update_from_current_config, true, true);
            }

            m_component.base_update_from_current_config();

            if (m_component.is_function_defined(ExComponent.Function.update_from_current_config)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.update_from_current_config, false, true);
            }
        }

        public void pre_start_routine() {

            if (m_component.is_function_defined(ExComponent.Function.pre_start_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.pre_start_routine, true, true);
            }

            m_component.base_pre_start_routine();

            if (m_component.is_function_defined(ExComponent.Function.pre_start_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.pre_start_routine, false, true);
            }
        }

        public void start_routine() {

            if (m_component.is_function_defined(ExComponent.Function.start_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.start_routine, true, true);
            }

            m_component.base_start_routine();

            if (m_component.is_function_defined(ExComponent.Function.start_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.start_routine, false, true);
            }
        }

        public void post_start_routine() {

            if (m_component.is_function_defined(ExComponent.Function.post_start_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.post_start_routine, true, true);
            }

            m_component.base_post_start_routine();

            if (m_component.is_function_defined(ExComponent.Function.post_start_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.post_start_routine, false, true);
            }
        }

        public void on_gui() {

            if (!m_component.is_updating()) {
                return;
            }

            if (m_component.is_function_defined(ExComponent.Function.on_gui)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.on_gui, true);
            }

            m_component.base_on_gui();

            if (m_component.is_function_defined(ExComponent.Function.on_gui)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.on_gui, false);
            }
        }

        public void pre_update() {

            if (!m_component.is_updating()) {
                return;
            }

            if (m_component.is_function_defined(ExComponent.Function.pre_update)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.pre_update, true);
            }
            m_component.base_pre_update();

            if (m_component.is_function_defined(ExComponent.Function.pre_update)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.pre_update, false);
            }

        }

        public void update() {

            if (!m_component.is_updating()) {
                return;
            }

            if (m_component.is_function_defined(ExComponent.Function.update)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.update, true);
            }
            m_component.base_update();

            if (m_component.is_function_defined(ExComponent.Function.update)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.update, false);
            }
        }

        public void post_update() {

            if (!m_component.is_updating()) {
                return;
            }

            if (m_component.is_function_defined(ExComponent.Function.post_update)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.post_update, true);
            }
            m_component.base_post_update();

            if (m_component.is_function_defined(ExComponent.Function.post_update)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.post_update, false);
            }
        }

        public void play() {

            if (!m_component.is_updating()) {
                return;
            }

            if (m_component.is_function_defined(ExComponent.Function.play)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.play, true, true, true, true);
            }
            m_component.base_play();

            if (m_component.is_function_defined(ExComponent.Function.play)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.play, false, true, true, true);
            }
        }

        public void pause() {

            if (!m_component.is_updating()) {
                return;
            }

            if (m_component.is_function_defined(ExComponent.Function.pause)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.pause, true, true, true, true);
            }
            m_component.base_pause();

            if (m_component.is_function_defined(ExComponent.Function.pause)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.pause, false, true, true, true);
            }
        }


        public void stop_routine() {

            if (m_component.is_function_defined(ExComponent.Function.stop_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.stop_routine, true, true);
            }

            m_component.base_stop_routine();

            if (m_component.is_function_defined(ExComponent.Function.stop_routine)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(m_component, ExComponent.Function.stop_routine, false, true);
            }
        }
    }
}