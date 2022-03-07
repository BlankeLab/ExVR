// system
using System;
using System.Reflection;
using System.Collections.Generic;
using System.Globalization;

// unity
using UnityEngine;

namespace Ex{

    // add target from another txt file
    // add VR hmd mesh in mirror


    public struct Traj{
        public Vector2 pos;
        public long time;
    }

    public class ReadDataScriptComponent : BaseCompiledCSharpComponent{


        public DateTime firstDate;
        public int currentId = 0;
        public List<Traj> data = new List<Traj>();

        public override bool initialize() {            

            // resources
            string theraTrainerTxt = init_config().get_resource_text("thera_txt");

            var lines = theraTrainerTxt.Split(new string[] { "\n" }, System.StringSplitOptions.None);
            var format = "HH.mm.ss.ffffff";
            int nbColumns = -1;            
            int currentLine = 0;
            foreach (var line in lines) {
                var split = line.Split(new string[] { " " }, System.StringSplitOptions.None);
                if (currentLine == 0) {
                    nbColumns = split.Length;
                    ++currentLine;
                    continue;
                }

                string dateStr = split[0];
                DateTime date = DateTime.Now;
                DateTime.TryParseExact(dateStr, format, CultureInfo.InvariantCulture, DateTimeStyles.None, out date);

                Traj t;
                t.pos = new Vector2(Ex.Converter.to_float(split[1]), Ex.Converter.to_float(split[2]));

                if (data.Count == 0) {
                    firstDate = date;
                    t.time = 0;
                } else {
                    var currDate = date - firstDate;
                    t.time = (long)currDate.TotalMilliseconds;
                }
                data.Add(t);
                ++currentLine;
            }

            return true;
        }


        public override void start_experiment() {
            currentId = 0;
        }

        public override void update() {

            long currentTime = ellapsed_time_routine_ms();

            for (int ii = currentId; ii < data.Count; ++ii) {

                long ms = data[ii].time;
                if (currentTime > ms) {
                    continue;
                }

                //log_message("curr time : " + currentTime + " " + ms + " " + ii + " " + data.Count);
                currentId = ii;
                invoke_signal1(data[ii]);
                break;
            }
        }
    }

    // skin color
    // male / female

    // 8 deviations cond
    // -30°, -22.5°, -15°, -7.5°, 7.5°, 15°, 22.5° et 30°
    // control cond: 0°

    // training
    // for12 routine1 -> no deviation
    // for18 -> 2 try for each deviation 

    // for76
    // 8 essais par déviation et 12 pour le contrôle -> 8*8=64 + 12 = 76

    // d0 d0 d0 d0 d0 d0 d0 d0 d0 d0 d0 d0 

    // 


    // d0 d0 d0 d0 d0 d0 d0 d0 d0 d0 d0 d0 
    // dn dn dn dn dn dn dn dn 


    //  Time-line:
    //  *12 essais d'entraînement sans déviation
    //  *18 essais d'entraînement incluant les 8 déviations + condition contrôle (2 essais par déviation présentés aléatoirement)
    //  *76 essais comprenant 8 essais par déviations et 12 essais pour la condition contrôle présentés aléatoirement.


    // target in front of avatar?

    public class TheraTrainerModelComponent : BaseCompiledCSharpComponent{

        AssetBundleComponent m_avatarEye    = null;
        AssetBundleComponent m_avatarMirror = null;

        AssetBundleComponent m_theraTrainerEye    = null;
        AssetBundleComponent m_theraTrainerMirror = null;

        HumanoidControllerComponent m_controllerEye     = null;
        HumanoidControllerComponent m_controllerMirror  = null;

        TheraTrainer m_theraEye     = null;
        TheraTrainer m_theraMirror  = null;

        MirrorComponent m_mirror = null;

        // ### Ex component flow functions, uncomment to use
        // # main functions
        public override bool initialize() {
            
            // components
            m_avatarEye             = init_config().get_component<AssetBundleComponent>("avatar_eye");
            m_avatarMirror          = init_config().get_component<AssetBundleComponent>("avatar_mirror");
            m_theraTrainerEye       = init_config().get_component<AssetBundleComponent>("thera_trainer_eye");
            m_theraTrainerMirror    = init_config().get_component<AssetBundleComponent>("thera_trainer_mirror");
            m_controllerEye         = init_config().get_component<HumanoidControllerComponent>("controller_eye");
            m_controllerMirror      = init_config().get_component<HumanoidControllerComponent>("controller_mirror");
            m_mirror                = init_config().get_component<MirrorComponent>("mirror");

            // retrieve thera trainer scripts
            m_theraEye      = m_theraTrainerEye.bundle.GetComponent<TheraTrainer>();
            m_theraMirror   = m_theraTrainerMirror.bundle.GetComponent<TheraTrainer>();
            

            if (m_avatarEye == null) {
                log_error("No avatar 1 set.");
                return false;
            }
            if (m_avatarMirror == null) {
                log_error("No avatar 2 set.");
                return false;
            }
            if (m_mirror == null) {
                log_error("No mirror set.");
                return false;
            }

            // layers
            ExVR.VR().apply_eye_camera_only_layer(m_avatarEye.gameObject);
            ExVR.VR().apply_eye_camera_only_layer(m_theraTrainerEye.gameObject);

            m_mirror.apply_mirror_only_layer(m_avatarMirror.gameObject);
            m_mirror.apply_mirror_only_layer(m_theraTrainerMirror.gameObject);

            return true;
        }

        public override void start_experiment() {
            m_mirror.apply_mirror_only_layer(ExVR.VR().debugVrHmd);
        }

        public override void start_routine() {
            
            m_controllerEye.set_left_arm_target(m_theraEye.leftHandGrip.transform);
            m_controllerEye.set_right_arm_target(m_theraEye.rightHandGrip.transform);
            m_controllerEye.set_pelvis_target(m_theraEye.pelvis.transform);

            m_controllerMirror.set_left_arm_target(m_theraMirror.leftHandGrip.transform);
            m_controllerMirror.set_right_arm_target(m_theraMirror.rightHandGrip.transform);
            m_controllerMirror.set_pelvis_target(m_theraMirror.pelvis.transform);

            update_from_current_config();
        }


        public override void update_parameter_from_gui(XML.Arg arg) {
            update_from_current_config();
        }

        public override void update_from_current_config() {

            Vector2 rot = current_config().get_vector2("thera_rotation");
            m_theraEye.rotX = rot.x;
            m_theraEye.rotY = rot.y;
            m_theraMirror.rotX = -rot.x;
            m_theraMirror.rotY = -rot.y;
        }

        // # slots
        public override void slot1(object value) {

            var t = (Traj)value;
           
            double angX = (180 / Math.PI) * Math.Atan((double)t.pos.x*0.01 / 0.6);
            double angY = (180 / Math.PI) * Math.Atan((double)t.pos.y* 0.01 / 0.6);

            m_theraEye.rotX = (float)angX;
            m_theraEye.rotY = (float)angY;

            m_theraMirror.rotX = -(float)angX;
            m_theraMirror.rotY = -(float)angY;

            //t.pos
            //log_message("pos: " + t.pos + " " + angX + " " + angY);
        }

        public override void slot5(IdAny idValue) {
            //int id = idValue.id;
            //object value = idValue.value;
            //log_message("Id: " + id);
            //if(id == 0) {
            //    log_message("int value: " + (int)value); 
            //}else if(id == 1) {
            //    log_message("real value: " + (double)value);
            //} else if(id == 2) {
            //    log_message("string value: " + (string)value);
            //}
        }
    }
}

