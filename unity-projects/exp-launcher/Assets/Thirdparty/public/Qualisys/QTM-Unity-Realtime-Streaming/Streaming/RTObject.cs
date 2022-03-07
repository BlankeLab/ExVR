// Unity SDK for Qualisys Track Manager. Copyright 2015 Qualisys AB
//
using UnityEngine;
using System.Collections;
using QTMRealTimeSDK;
using System.Collections.Generic;

namespace QualisysRealTime.Unity
{
    class RTObject : MonoBehaviour
    {
        public string ObjectName = "Put QTM 6DOF object name here";
        public Vector3 PositionOffset = new Vector3(0, 0, 0);
        public Vector3 EulerOffset = new Vector3(0, 0, 0);

        public Vector3 v3_final_position_multiplier = new Vector3(1, 1, 1); //Vector3(-1, 1, 1)

        private RTClient rtClient;

        // Use this for initialization
        void Start()
        {
            rtClient = RTClient.GetInstance();
        }

        // Update is called once per frame
        void LateUpdate()
        {
            if (b_debug_add_delay)
            {
                b_debug_add_delay = false;
                add_delay(i_debug_frames_delay);
            }

            if (rtClient == null) rtClient = RTClient.GetInstance();

            SixDOFBody body = rtClient.GetBody(ObjectName);
            if (body != null)
            {
                if (body.Position.magnitude > 0) //just to avoid error when position is NaN
                {
                    transform.position = transform.parent.rotation * (body.Position + PositionOffset) + transform.parent.position;
                    transform.localRotation = body.Rotation * Quaternion.Euler(EulerOffset);
                }
                /*if (ObjectName == "RBStick")
                {
                    if (!float.IsNaN(rtClient.GetBody("RBbellyYcalibration").Position.y))
                    {
                        transform.position -= new Vector3(0, rtClient.GetBody("RBbellyYcalibration").Position.y, 0);
                        transform.position += new Vector3(0, 0.15f, 0); // avatar default belly scale
                    }
                }*/
            }


            if (i_frames_to_wait > 0)
            {
                list_v3_position_delay.Add(transform.position);
                transform.position = list_v3_position_delay[0];
                list_v3_position_delay.RemoveAt(0);
                list_v3_rotation_delay.Add(transform.rotation);
                transform.rotation = list_v3_rotation_delay[0];
                list_v3_rotation_delay.RemoveAt(0);
            }

            if (v3_final_position_multiplier != new Vector3(1, 1, 1))
            {
                this.transform.localPosition = new Vector3(this.transform.localPosition.x * v3_final_position_multiplier.x, this.transform.localPosition.y * v3_final_position_multiplier.y, this.transform.localPosition.z * v3_final_position_multiplier.z);

                if (v3_final_position_multiplier.x == -1)
                {
                    Quaternion Q_ = this.transform.localRotation;
                    this.transform.localRotation = new Quaternion(this.transform.localRotation.x * -1.0f,
                                    this.transform.localRotation.y,
                                    this.transform.localRotation.z,
                                    this.transform.localRotation.w * -1.0f);
                }
                else if (v3_final_position_multiplier.y == -1)
                {
                    Quaternion Q_ = this.transform.localRotation;
                    this.transform.localRotation = new Quaternion(this.transform.localRotation.x,
                                    this.transform.localRotation.y * -1.0f,
                                    this.transform.localRotation.z,
                                    this.transform.localRotation.w * -1.0f);
                }
                else if (v3_final_position_multiplier.z == -1)
                {
                    Quaternion Q_ = this.transform.localRotation;
                    this.transform.localRotation = new Quaternion(this.transform.localRotation.x,
                                    this.transform.localRotation.y,
                                    this.transform.localRotation.z* -1.0f,
                                    this.transform.localRotation.w * -1.0f);
                }
            }
        }

        List<Vector3> list_v3_position_delay = new List<Vector3>();
        List<Quaternion> list_v3_rotation_delay = new List<Quaternion>();
        [HideInInspector]
        int i_frames_to_wait;
        public void add_delay(int i_frames)
        {
            i_frames_to_wait = i_frames;
            list_v3_position_delay.Clear();
            list_v3_rotation_delay.Clear();
            list_v3_position_delay = new List<Vector3>();
            list_v3_rotation_delay = new List<Quaternion>();
            while (list_v3_position_delay.Count < i_frames_to_wait)
            {
                list_v3_position_delay.Add(transform.position);
                list_v3_rotation_delay.Add(transform.rotation);
            }
        }

        public bool b_debug_add_delay;
        public int i_debug_frames_delay = 60;
    }
}