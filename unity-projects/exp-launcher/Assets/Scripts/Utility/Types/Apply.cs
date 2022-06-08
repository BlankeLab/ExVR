

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


// unity
using UnityEngine;

namespace Ex {

    public static class Apply {

        public static void to_transform(TransformValue trV, Transform transform, bool local, bool applyPosition = true, bool applyRotation = true, bool applyScale = true) {

            if (applyScale) {
                transform.localScale = trV.scale;
            }

            if (local) {
                if (applyPosition) {
                    transform.localPosition = trV.position;
                }
                if (applyRotation) {
                    transform.localRotation = trV.rotation;
                }
            } else {
                if (applyPosition) {
                    transform.position = trV.position;
                }
                if (applyRotation) {
                    transform.rotation = trV.rotation;
                }
            }
        }

        public static void to_transform(object value, Transform transform, bool local, bool applyPosition, bool applyRotation, bool applyScale) {

            if (value is TransformValue) {
                to_transform((TransformValue)value, transform, local, applyPosition, applyRotation, applyScale);
                return;
            }

            var values =  Converter.to_list<float>(value);
            if (applyScale) {
                transform.localScale = new Vector3(values[6], values[7], values[8]);
            }

            if (local) {
                if (applyPosition) {
                    transform.localPosition = new Vector3(values[0], values[1], values[2]);
                }
                if (applyRotation) {
                    //transform.localEulerAngles = new Vector3(values[3], values[4], values[5]);
                    transform.localRotation =
                        Quaternion.AngleAxis(values[4], Vector3.up) *
                        Quaternion.AngleAxis(values[3], Vector3.right) *
                        Quaternion.AngleAxis(values[5], Vector3.forward);
                }
            } else {
                if (applyPosition) {
                    transform.position = new Vector3(values[0], values[1], values[2]);
                }
                if (applyRotation) {
                    //transform.eulerAngles = new Vector3(values[3], values[4], values[5]);
                    transform.rotation =
                        Quaternion.AngleAxis(values[4], Vector3.up) *
                        Quaternion.AngleAxis(values[3], Vector3.right) *
                        Quaternion.AngleAxis(values[5], Vector3.forward);
                }
            }
        }

    }
}


