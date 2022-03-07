// Unity SDK for Qualisys Track Manager. Copyright 2015 Qualisys AB
//
using UnityEngine;

namespace Ex { 
    public class QualisysTrackedBody : MonoBehaviour{

        public bool tracked = false;
        public int currentFrame = 0;

        private QualisysRealTime.Unity.RTClient rtClient;

        void Start() {
            rtClient = QualisysRealTime.Unity.RTClient.GetInstance();
        }

        void LateUpdate() {
            
            // init client
            if (rtClient == null) {
                rtClient = QualisysRealTime.Unity.RTClient.GetInstance();
            }

            // get current frame
            currentFrame = rtClient.GetFrame();

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