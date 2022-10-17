
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
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace Ex{

    namespace XML{

        // ######################################## expriment xml

        [XmlRoot(ElementName = "Arg")]
        public class Arg{

            // attributes
            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "ui")] // only used in designer
            public string UiGeneratorType { get; set; }

            [XmlAttribute(AttributeName = "value")]
            public string Value { get; set; }

            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }

            [XmlAttribute(AttributeName = "dim")]
            public int Dim { get; set; }

            [XmlAttribute(AttributeName = "sep")]
            public string Separator { get; set; }

            [XmlAttribute(AttributeName = "sizes")]
            public string Sizes { get; set; }
        }

        [XmlRoot(ElementName = "InitConfig")]
        public class InitConfig{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            // elements
            [XmlElement(ElementName = "Arg")]
            public List<Arg> Arg { get; set; }
        }

        [XmlRoot(ElementName = "Config")]
        public class Config{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            // elements
            [XmlElement(ElementName = "Arg")]
            public List<Arg> Arg { get; set; }
        }

        [XmlRoot(ElementName = "Configs")]
        public class Configs{

            // elements
            [XmlElement(ElementName = "Config")]
            public List<Config> Config { get; set; }
        }

        [XmlRoot(ElementName = "Component")]
        public class Component{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "category")]
            public string Category { get; set; }

            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }

            [XmlAttribute(AttributeName = "global")]
            public bool Global { get; set; }

            [XmlAttribute(AttributeName = "always_updating")]
            public bool AlwaysUpdating { get; set; }

            [XmlAttribute(AttributeName = "exceptions")]
            public bool Exceptions { get; set; }

            [XmlAttribute(AttributeName = "frame_logging")]
            public bool FrameLogging { get; set; }

            [XmlAttribute(AttributeName = "trigger_logging")]
            public bool TriggerLogging { get; set; }

            [XmlAttribute(AttributeName = "restricted")]
            public int Restricted { get; set; }

            [XmlAttribute(AttributeName = "priority")]
            public int Priority { get; set; }

            // elements
            [XmlElement(ElementName = "InitConfig")]
            public InitConfig InitConfig { get; set; }

            [XmlElement(ElementName = "Configs")]
            public Configs Configs { get; set; }
        }

        [XmlRoot(ElementName = "Components")]
        public class Components{

            // elements
            [XmlElement(ElementName = "Component")]
            public List<Component> Component { get; set; }
        }

        [XmlRoot(ElementName = "Interval")]
        public class Interval{

            // attributes
            [XmlAttribute(AttributeName = "t1")]
            public double T1 { get; set; }

            [XmlAttribute(AttributeName = "t2")]
            public double T2 { get; set; }
        }

        [XmlRoot(ElementName = "Timeline")]
        public class Timeline{

            // attributes
            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }

            // elements
            [XmlElement(ElementName = "Interval")]
            public List<Interval> Interval { get; set; }
        }

        [XmlRoot(ElementName = "Action")]
        public class Action{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "key_component")]
            public int KeyComponent { get; set; }

            [XmlAttribute(AttributeName = "key_config")]
            public int KeyConfig { get; set; }

            [XmlAttribute(AttributeName = "node_used")]
            public int NodeUsed { get; set; }

            [XmlAttribute(AttributeName = "node_position")]
            public string Position { get; set; }

            // elements
            [XmlElement(ElementName = "Timeline")]
            public List<Timeline> Timelines { get; set; }
        }

        [XmlRoot(ElementName = "Connection")]
        public class Connection{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            // start
            [XmlAttribute(AttributeName = "out_type")]
            public string StartType { get; set; }

            [XmlAttribute(AttributeName = "out_key")]
            public int StartKey { get; set; }
            
            [XmlAttribute(AttributeName = "signal_id")]
            public int StartIndex { get; set; }

            [XmlAttribute(AttributeName = "signal")]
            public string Signal { get; set; }

            [XmlAttribute(AttributeName = "out_data_type")]
            public string StartDataType { get; set; }

            // end
            [XmlAttribute(AttributeName = "in_type")]
            public string EndType { get; set; }

            [XmlAttribute(AttributeName = "in_key")]
            public int EndKey { get; set; }

            [XmlAttribute(AttributeName = "slot_id")]
            public int EndIndex { get; set; }

            [XmlAttribute(AttributeName = "slot")]
            public string Slot { get; set; }

            [XmlAttribute(AttributeName = "in_data_type")]
            public string EndDataType { get; set; }
        }

        [XmlRoot(ElementName = "Connector")]
        public class Connector{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "node_position")]
            public string Position { get; set; }

            // elements
            [XmlElement(ElementName = "Arg")]
            public List<Arg> Arg { get; set; }
        }


        [XmlRoot(ElementName = "Condition")]
        public class Condition{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "duration")]
            public double Duration { get; set; }

            // elements
            [XmlElement(ElementName = "Action")]
            public List<Action> Actions { get; set; }

            [XmlElement(ElementName = "Connector")]
            public List<Connector> Connectors { get; set; }

            [XmlElement(ElementName = "Connection")]
            public List<Connection> Connections { get; set; }
        }

        [XmlRoot(ElementName = "Routine")]
        public class Routine{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "randomizer")]
            public bool Randomizer { get; set; }

            // elements
            [XmlElement(ElementName = "Condition")]
            public List<Condition> Conditions { get; set; }
        }

        [XmlRoot(ElementName = "Routines")]
        public class Routines{

            // elements
            [XmlElement(ElementName = "Routine")]
            public List<Routine> Routine { get; set; }
        }

        [XmlRoot(ElementName = "Isi")]
        public class Isi{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "set")]
            public string Set { get; set; }

            [XmlAttribute(AttributeName = "randomized")]
            public bool Randomized { get; set; }
        }

        [XmlRoot(ElementName = "ISIs")]
        public class ISIs{

            // elements
            [XmlElement(ElementName = "Isi")]
            public List<Isi> Isi { get; set; }
        }


        [XmlRoot(ElementName = "Set")]
        public class Set {

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "occu")]
            public int Occurency { get; set; }
        }

        [XmlRoot(ElementName = "Loop")]
        public class Loop{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }

            [XmlAttribute(AttributeName = "nbReps")]
            public int NbReps { get; set; }

            // elements
            [XmlElement(ElementName = "Set")]
            public List<Set> Set { get; set; }
        }

        [XmlRoot(ElementName = "Loops")]
        public class Loops{

            // elements
            [XmlElement(ElementName = "Loop")]
            public List<Loop> Loop { get; set; }
        }

        [XmlRoot(ElementName = "Element")]
        public class ElementSeq{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }
        }

        [XmlRoot(ElementName = "FlowSequence")]
        public class FlowSequence{

            // elements
            [XmlElement(ElementName = "Element")]
            public List<ElementSeq> Element { get; set; }
        }

        [XmlRoot(ElementName = "FlowElements")]
        public class FlowElements{

            // elements
            [XmlElement(ElementName = "Loops")]
            public Loops Loops { get; set; }

            [XmlElement(ElementName = "Routines")]
            public Routines Routines { get; set; }

            [XmlElement(ElementName = "ISIs")]
            public ISIs ISIs { get; set; }
        }

        [XmlRoot(ElementName = "Display")]
        public class Display{

            // attributes
            [XmlAttribute(AttributeName = "mode")]
            public int Mode { get; set; }

            [XmlAttribute(AttributeName = "stereo_fov")]
            public int StereoFOV { get; set; }
        }

        [XmlRoot(ElementName = "Camera")]
        public class Camera
        {
            [XmlAttribute(AttributeName = "neutral_x")]
            public bool CameraNeutralX { get; set; }

            [XmlAttribute(AttributeName = "neutral_y")]
            public bool CameraNeutralY { get; set; }

            [XmlAttribute(AttributeName = "neutral_z")]
            public bool CameraNeutralZ { get; set; }
        }

        [XmlRoot(ElementName = "Settings")]
        public class Settings{

            // attributes
            [XmlAttribute(AttributeName = "debug")]
            public bool Debug { get; set; }

            [XmlAttribute(AttributeName = "csharp_debug_info")]
            public bool CSharpDebugInfo { get; set; }

            [XmlAttribute(AttributeName = "catch_components_exceptions")]
            public bool CatchComponentsExceptions { get; set; }

            [XmlAttribute(AttributeName = "positional_tracking")]
            public bool PositionalTracking { get; set; }

            [XmlAttribute(AttributeName = "catch_external_keyboard_events")]
            public bool CatchExternalKeyboardEvents { get; set; }            

            [XmlElement(ElementName = "Display")]
            public Display Display { get; set; }

            [XmlElement(ElementName = "Camera")]
            public Camera Camera { get; set; }
        }

        [XmlRoot(ElementName = "Resource")]
        public class Resource{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }

            [XmlAttribute(AttributeName = "alias")]
            public string Alias { get; set; }

            [XmlAttribute(AttributeName = "path")]
            public string Path { get; set; }
        }

        [XmlRoot(ElementName = "Resources")]
        public class Resources{

            // attributes
            [XmlAttribute(AttributeName = "reload")]
            public int ReloadCode { get; set; }

            // elements
            [XmlElement(ElementName = "Resource")]
            public List<Resource> Resource { get; set; }
        }

        [XmlRoot(ElementName = "Experiment")]
        public class Experiment{

            // attributes
            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "version")]
            public string Version { get; set; }

            [XmlAttribute(AttributeName = "designer-used")]
            public string DesignerUsed { get; set; }

            // elements
            [XmlElement(ElementName = "Settings")]
            public Settings Settings { get; set; }

            [XmlElement(ElementName = "Resources")]
            public Resources Resources { get; set; }

            [XmlElement(ElementName = "Components")]
            public Components Components { get; set; }

            [XmlElement(ElementName = "FlowElements")]
            public FlowElements FlowElements { get; set; }

            [XmlElement(ElementName = "FlowSequence")]
            public FlowSequence FlowSequence { get; set; }
        }

        // ######################################## instance xml
        [XmlRoot(ElementName = "Element")]
        public class Element{

            // attributes
            [XmlAttribute(AttributeName = "key")]
            public int Key { get; set; }

            [XmlAttribute(AttributeName = "type")]
            public string Type { get; set; }

            [XmlAttribute(AttributeName = "name")]
            public string Name { get; set; }

            [XmlAttribute(AttributeName = "cond")]
            public string Cond { get; set; }

            [XmlAttribute(AttributeName = "elem_iter")]
            public int ElemIteration { get; set; }

            [XmlAttribute(AttributeName = "cond_iter")]
            public int ConditionIteration { get; set; }
        }

        [XmlRoot(ElementName = "ExperimentFlow")]
        public class ExperimentFlow{

            // attributes
            [XmlAttribute(AttributeName = "id_instance")]
            public int IdInstance { get; set; }

            // elements
            [XmlElement(ElementName = "Element")]
            public List<Element> Elements { get; set; }
        }
    }
}