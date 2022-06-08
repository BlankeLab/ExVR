
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

//using UnityEngine;

//public class AnimationController : MonoBehaviour{

//    private Animator m_animator = null;

//    [SerializeField]
//    public string state = "Default";

//    [SerializeField]
//    public bool auto = false;

//    [SerializeField]
//    [Range(0.0f, 1.0f)]
//    public float time = 0f;

//    [SerializeField]
//    [Range(0.0f, 10f)]
//    public float factor = 1f;

//    // Start is called before the first frame update
//    void Awake(){
//        m_animator = GetComponent<Animator>();
//    }

//    // Update is called once per frame
//    void Update(){
//        return;
//        if (auto) {
//            m_animator.speed = 1f;
//            time += factor * Time.deltaTime;
//            if (time > 1f) {
//                time -= 1f;
//            }

//        } else {
//            m_animator.speed = 0f;
//        }

//        m_animator.Play(state, 0, time);
//    }

//    public void set_current_state(float time) {
//        this.time = time;
//        m_animator.Play(state, 0, time);
//    }
//}
////using System.Collections;
////using System.Collections.Generic;
////using UnityEngine;

////public class controllAnim : MonoBehaviour
////{

////    public List<GameObject> keepValues;

////    private List<Transform> transfos;
////    private List<Vector3> positions;
////    private List<Quaternion> rotations;

////    // Start is called before the first frame update
////    void Awake() {

////        positions = new List<Vector3>();
////        rotations = new List<Quaternion>();
////        transfos = new List<Transform>();
////        foreach (var go in keepValues) {
////            Transform[] trInChildren = go.GetComponentsInChildren<Transform>();
////            foreach (var tr in trInChildren) {
////                transfos.Add(tr);
////                positions.Add(tr.position);
////                rotations.Add(tr.rotation);
////            }
////        }
////    }

////    // Update is called once per frame
////    void Update() {

////        if (auto) {
////            ratio += factor * Time.deltaTime;
////            if (ratio > 1f) {
////                ratio -= 1f;
////            }
////        }
////        GetComponent<Animator>().SetFloat("progress", ratio);

////        for (int ii = 0; ii < transfos.Count; ++ii) {
////            transfos[ii].position = positions[ii];
////            transfos[ii].rotation = rotations[ii];
////        }
////    }
////}
