

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
using System;
using System.Diagnostics;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex {

    public class FPSCounter : MonoBehaviour {

        TMPro.TextMeshProUGUI text = null;
        Queue<Tuple<long, float>> lastValues = new Queue<Tuple<long, float>>();
        Stopwatch sw = new Stopwatch();

        private Color32 veryLow = Color.red;
        private Color32 low     = Color.yellow;
        private Color32 ok      = Color.green;

        private void Start() {
            text = GetComponent<TMPro.TextMeshProUGUI>();
            sw.Start();
        }
        private void Update() {

            var currentTime = sw.ElapsedMilliseconds;
            float fps = 1f / Time.unscaledDeltaTime;

            lastValues.Enqueue(new Tuple<long, float>(currentTime, fps));
            bool removeFirst = (currentTime - lastValues.Peek().Item1) > 1000;

            while (removeFirst) {
                lastValues.Dequeue();
                removeFirst = (currentTime - lastValues.Peek().Item1) > 1000;
            }

            float total = 0f;
            foreach (var value in lastValues) {
                total += value.Item2;
            }
            total /= lastValues.Count;

            int currenRate = Screen.currentResolution.refreshRate;
            text.SetText(Converter.to_string(total, total < 100 ? "00.0" : "000."));
            if (total > currenRate * 0.95f) {
                text.faceColor = ok;
                text.outlineColor = ok;
            } else if (total > currenRate * 0.8f) {
                text.faceColor = low;
                text.outlineColor = low;
            } else {
                text.faceColor = veryLow;
                text.outlineColor = veryLow;
            }
        }
    }
}