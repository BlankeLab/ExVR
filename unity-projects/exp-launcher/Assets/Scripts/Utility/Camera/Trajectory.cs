
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
using System.Text.RegularExpressions;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class Interpolate{
        public static Quaternion rotation(Quaternion q1, Quaternion q2, float t, bool sphericalInterpolation) {

            if (sphericalInterpolation) {
                return Quaternion.Slerp(q1, q2, t);
            }
            return Quaternion.Lerp(q1, q2, t);
        }

        public static Vector3 vector(Vector3 v1, Vector3 v2, float t, bool sphericalInterpolation) {

            if (sphericalInterpolation) {
                return Vector3.Slerp(v1, v2, t);
            }
            return Vector3.Lerp(v1, v2, t);
        }
    }

    public class Trajectory{

        public List<Vector3> positions = new List<Vector3>();
        public List<Quaternion> rotations = new List<Quaternion>();
        public List<float> factors = null;

        public int count() {
            return rotations.Count;
        }

        public void add_trajectory_point(Transform tr) {
            rotations.Add(tr.rotation);
            positions.Add(tr.position);
        }

        public void add_trajectory_point(Vector3 position, Quaternion rotation) {
            rotations.Add(rotation);
            positions.Add(position);
        }

        public void apply_to_transform(Transform tr, float percent) {
            int id = (int)((count() - 1) * percent);
            tr.rotation = rotations[id];
            tr.position = positions[id];
        }

        public Vector3 get_position(float percent) {
            if(count() == 0) {
                return Vector3.zero;
            }
            int id = (int)((count() - 1) * percent);
            return positions[id];
        }

        public Quaternion get_rotation(float percent) {

            if (count() == 0) {
                return Quaternion.identity;
            }
            int id = (int)((count() - 1) * percent);
            return rotations[id];
        }

        public Vector3 last_position() {
            return (positions.Count > 0) ? positions[positions.Count - 1] : Vector3.zero;
        }
        public Quaternion last_rotation() {
            return (rotations.Count > 0) ? rotations[rotations.Count - 1] : Quaternion.identity;
        }


        public bool init_from_text(string content, bool inverse) {

            var lines = content.Split('\n');
            if (lines.Length < 2) {
                ExVR.Log().error("Trajectory needs at least two points.");
                return false;
            }

            positions = new List<Vector3>(lines.Length);
            rotations = new List<Quaternion>(lines.Length);
            factors   = new List<float>(lines.Length - 1);

            foreach (var line in lines) {

                var split = Regex.Replace(line, @"\s+", " ").Split(' ');
                if (split.Length >= 3) {
                    positions.Add(new Vector3(Converter.to_float(split[0]), Converter.to_float(split[1]), Converter.to_float(split[2])));
                }
                if (split.Length >= 6) {
                    rotations.Add(Quaternion.Euler(Converter.to_float(split[3]), Converter.to_float(split[4]), Converter.to_float(split[5])));
                }
            }

            if (inverse) {
                positions.Reverse();
                rotations.Reverse();
            }

            float totalDist = 0f;
            List<float> distances = new List<float>(positions.Count - 1);
            for (int ii = 1; ii < positions.Count; ++ii) {
                float d = (positions[ii - 1] - positions[ii]).magnitude;
                totalDist += d;
                distances.Add(d);
            }

            for (int ii = 0; ii < distances.Count; ++ii) {
                factors.Add(distances[ii] / totalDist);
            }

            return true;
        }

        public Vector3 compute_position(float currentTime, float totalTime, bool sphericalInterpolation) {
            
            if (currentTime > totalTime) {
                return last_position();
            }

            float pIdF = (currentTime / totalTime) * (count() - 1);
            int integerPart = (int)Math.Floor(pIdF);
            float floatPart = (float)(pIdF - Math.Truncate(pIdF));

            if (integerPart < count()) {
                return Interpolate.vector(positions[integerPart], positions[integerPart + 1], floatPart, sphericalInterpolation);
            }
            return last_position();
        }

        public Quaternion compute_rotation(float currentTime, float totalTime, bool sphericalInterpolation) {

            if (currentTime > totalTime) {
                return last_rotation();
            }

            float pIdF = (currentTime / totalTime) * (count() - 1);
            int integerPart = (int)Math.Floor(pIdF);
            float floatPart = (float)(pIdF - Math.Truncate(pIdF));

            if (integerPart < count()) {
                return Interpolate.rotation(rotations[integerPart], rotations[integerPart + 1], floatPart, sphericalInterpolation);
            }

            return last_rotation();
        }
    }
}