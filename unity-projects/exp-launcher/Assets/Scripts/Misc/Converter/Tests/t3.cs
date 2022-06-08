using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace Test
{
    [Serializable]
    public struct t4
    {
        public List<Transform> e;
        public List<GameObject> f;
        public List<t2> g;
    }

    public class t3 : MonoBehaviour
    {
        public List<bool> a;
        public List<int> b;
        public List<float> c;
        public List<Vector3> d;
        public List<Transform> e;
        public List<GameObject> f;
        public List<t2> g;
        

        public bool[] h;
        public float[] i;
        public Vector3[] j;
        public Transform[] k;
        public GameObject[] l;
        public t1[] m;
        public t4 n;

        void Awake() {
            Debug.Log("T3::AWAKE");
        }

        // Start is called before the first frame update
        void Start() {
            Debug.Log("T3::START");
        }

    }
}