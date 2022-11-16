
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
using System.Collections.Generic;

namespace Ex {

    public class ForceComponentConfigConnector : ExConnector {

        protected override bool initialize() {
            add_slot(0, (nullArg) => { base_slot1(null);});
            return true;
        }

        protected override void slot1(object arg) {

            var value = m_config.get(valueStr);
            if (value == null) {
                return;
            }

            var split = ((List<object>)value.value);
            if (split.Count != 6) {
                return;
            }

            Routine routine         = null;
            Condition condition     = null;
            ExComponent component   = null;
            ComponentConfig config  = null;

            string routineName = (string)split[2];
            routine = ExVR.Routines().get(routineName, routineName.Length != 0);
            if(routineName.Length != 0 && routine == null) {
                log_error(string.Format("Invalid routine name: {0}", routineName));
                return;
            }

            string conditionName = (string)split[3];
            if (routine != null) {
                condition = routine.get_condition_from_name(conditionName, conditionName.Length != 0);
                if(conditionName.Length != 0 && condition == null) {
                    log_error(string.Format("Invalid condition name: {0} from routine: {1}", conditionName, routineName));
                    return;
                }
            }

            int componentKey = Converter.to_int((string)split[4]);
            int configKey    = Converter.to_int((string)split[5]);

            if (componentKey != -1) {
                component = ExVR.Components().get_from_key(componentKey);
            }
            if ((component != null) && (configKey != -1)) {
                config = component.get_config(configKey);
            }

            if (component == null) {
                log_error(string.Format("Cannot retrieve component from key: {0}", componentKey));
                return;
            }
            if (config == null) {
                log_error(string.Format("Cannot retrieve config from infos: {0}", configKey));
                return;
            }

            if(routine == null) {

                for (int ii = 0; ii < ExVR.Routines().count(); ++ii) {

                    var currentR = ExVR.Routines().get_from_id(ii);
                    if (conditionName.Length > 0) {
                        var currentC = currentR.get_condition_from_name(conditionName, false);
                        if (currentC != null) {
                            var action = currentC.get_action_from_component_key(component.key);
                            if (action != null) {
                                action.force_config(config);
                            }
                        }
                    } else {
                        currentR.force_config_to_all_conditions(component, config.name);
                    }
                }
           
            }else if(condition == null) {
                routine.force_config_to_all_conditions(component, config.name);
            } else if (condition != null) {
                var action = condition.get_action_from_component_key(component.key);
                if(action != null) {
                    action.force_config(config);                    
                }
            }

            component.update_from_current_config();
        }
    }
}