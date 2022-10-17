
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

    public class ExVR {

        static public GoExplorer GO() {
            return Builder.autoRef.GO();
        }
        static public GuiSettingsManager GuiSettings() {
            return Builder.autoRef.guiSettingsManager;
        }

        static public DisplayManager Display() {
            return Builder.autoRef.displayManager;
        }

        static public TimeManager Time() {
            return Builder.autoRef.timeManager;
        }

        static public PathsManager Paths() {
            return Builder.autoRef.pathsManager;
        }

        static public CoroutinesManager Coroutines() {
            return Builder.autoRef.coroutinesManager;
        }

        static public EventsManager Events() {
            return Builder.autoRef.eventsManager;
        }

        static public GlobalResourcesManager GlobalResources() {
            return Builder.autoRef.globalResourcesManager;
        }

        static public NetworkManager Network() {
            return Builder.autoRef.networkManager;
        }

        static public LoggerManager Logger() {
            return Builder.autoRef.loggerManager;
        }

        static public Log Log() {
            return Logger().log;
        }

        static public ExperimentLogger ExpLog() {
            return Logger().exp;
        }

        static public PythonManager Python() {
            return Builder.autoRef.pythonManager;
        }

        static public MemoryManager Memory() {
            return Builder.autoRef.memoryManager;
        }


        // experiment
        static public Experiment Experiment() {
            return Builder.autoRef.experiment;
        }
        static public Scheduler Scheduler() {return Experiment().schreduler;}

        // # randomization
        static public Instance Instance() {return Scheduler().instance;}

        // # data
        // ## resources
        static public ResourcesManager Resources() {return Experiment().experimentResourcesManager;}
        // ## components
        static public Components Components() {return Experiment().components;}
        // ## elements
        static public Routines Routines() {return Experiment().routines;}
        static public ISIs ISIs() {return Experiment().ISIs;}
    }
}


