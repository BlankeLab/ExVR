using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace Test
{

    public class t1 : MonoBehaviour
    {
        public bool a;
        public bool b;
        public int c;
        public int d;
        public byte e;
        public long f;
        public long g;
        public float h;
        public float i;
        public double j;
        public double k;
        public string l;

        public Vector2 m;
        public Vector3 n;
        public Quaternion o;
        public Transform p = null;
        public GameObject q = null;
        public Image r = null; 

        void Awake() {
            Debug.Log("T1::AWAKE");
        }

        // Start is called before the first frame update
        void Start() {
            Debug.Log("T1::START");
        }

 
    }
}