// Unity SDK for Qualisys Track Manager. Copyright 2015 Qualisys AB
//
using UnityEngine;

namespace Ex { 
    public class QualisysTrackedBody : MonoBehaviour{

        public bool tracked = false;
        public int currentFrame = 0;

        private static QualisysRealTime.Unity.RTClient rtClient = null;

        public static void init() {
            rtClient = QualisysRealTime.Unity.RTClient.GetInstance();
        }

        public void update_frame() {

            if (rtClient == null) {
                return;
            }
            // get current frame
            currentFrame = rtClient.GetFrame();
        }

        public void update_body() {



            // reset tracking
            tracked = false;            

            var body = rtClient.GetBody(name);
            if (body != null) {
                if (body.Position.magnitude > 0) { //just to avoid error when position is NaN
                    transform.localPosition = body.Position;
                    transform.localRotation = body.Rotation;
                    tracked = true;
                }
            }
        }
    }
}