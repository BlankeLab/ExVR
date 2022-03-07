using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace Test
{
    public class E1
    {
        public enum EnumT1 { a, b, c, d };
        public int a;
    }

    [Serializable]
    public class E2
    {
        public int b;
    }

    [System.Serializable]
    public class t2 : MonoBehaviour
    {
        public enum EnumT2 { e,f,g,h};

        public E1.EnumT1 tE1;
        public EnumT2 tE2;
        public E1 classE1 = null;
        public E2 classE2 = null;

        void Awake() {
            Debug.Log("T1::AWAKE");
        }
        
        // Start is called before the first frame update
        void Start() {
            Debug.Log("T2::START");
        }


    }
}