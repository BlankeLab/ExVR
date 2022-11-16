
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
using System.IO;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{


    public class PlotResource : ExResourceFile{

        public List<AnimationCurve> curves = null;

        public override void create(int key, string alias, string path){

            base.create(key, alias, path);

            var ext = Path.GetExtension(path);
            if (ext == ".csv") {

                using (var reader = new StreamReader(path)) {

                    while (!reader.EndOfStream) {
                        var line = reader.ReadLine();
                        var values = line.Split(',');

                        if (values.Length < 2) {
                            // error
                            return;
                        }

                        if (curves == null) {
                            curves = new List<AnimationCurve>(values.Length - 1);
                            for (int ii = 0; ii < values.Length - 1; ++ii) {
                                curves.Add(new AnimationCurve());
                            }
                        }

                        float time = Converter.to_float(values[0]);
                        for (int ii = 0; ii < values.Length - 1; ++ii) {
                            curves[ii].AddKey(time, Converter.to_float(values[ii + 1]));
                        }
                    }
                }
            }

            if (curves != null) {
                //foreach (var curve in curves) {
                //    UnityEngine.Debug.Log("curve " + curve.length);
                //}
            } else {
                ExVR.Log().error("Cannot load plot file: " + alias + " with path: " + path);
            }
        }
    }
}