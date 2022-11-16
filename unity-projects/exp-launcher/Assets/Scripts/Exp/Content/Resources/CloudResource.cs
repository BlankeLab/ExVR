
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

namespace Ex {

    public class CloudResource : ExResourceFile{

        public List<Vector3> vertices = null;
        public List<Color> colors = null;

        public override void create(int key, string alias, string path){

            base.create(key, alias, path);

            string extension = System.IO.Path.GetExtension(path).ToLower();
            bool success = false;
            if (extension == ".asc") {
                success = load_asc_file(path);
            } else if (extension == ".ply") {
                success = load_ply_file(path);
            } else if (extension == ".obj") {
                success = load_obj_file(path);
            } else {
                log_error(string.Format("Cloud extension not managed: [{0}]", extension));
            }

            if (!success) {
                vertices = new List<Vector3>();
                colors = new List<Color>();
            }
        }

        private bool load_asc_file(string path) {


            string content;
            try {
                content = File.ReadAllText(path);
            } catch (System.Exception ex) {
                log_error(string.Format("Cannot read file with path [{0}], error: [{1}]", path, ex.Message));
                return false;
            }
            var lines = Text.split_lines(content);
            vertices = new List<Vector3>(lines.Length);
            colors   = new List<Color>(lines.Length);

            foreach (var line in lines) {
                var split = Text.split(line, ' ');
                if (split.Length == 3) {
                    vertices.Add(new Vector3(Converter.to_float(split[0]), Converter.to_float(split[1]), Converter.to_float(split[2])));
                    colors.Add(Color.white);
                }
                //colors.Add(new Color(Converter.to_int(split[3]) / 255f, Converter.to_int(split[4]) / 255f, Converter.to_int(split[5]) / 255f));
            }

            if (vertices.Count == 0) {
                log_error("Empty cloud.");
                return false;
            }

            return true;
        }
        private bool load_ply_file(string path) {

            string content;
            try {
                content = File.ReadAllText(path);
            } catch (System.Exception ex) {
                log_error(string.Format("Cannot read file with path [{0}], error: [{1}]", path, ex.Message));
                return false;
            }
            var lines = Text.split_lines(content);
            vertices = new List<Vector3>(lines.Length);
            colors = new List<Color>(lines.Length);

            foreach (var line in lines) {

            }

            if (vertices.Count == 0) {
                log_error("Empty cloud.");
                return false;
            }


            return true;
        }

        private bool load_obj_file(string path) {

            string content;
            try {
                content = File.ReadAllText(path);
            } catch (System.Exception ex) {
                log_error(string.Format("Cannot read file with path [{0}], error: [{1}]", path, ex.Message));
                return false;
            }
            var lines = Text.split_lines(content);
            vertices = new List<Vector3>(lines.Length);
            colors = new List<Color>(lines.Length);

            foreach (var line in lines) {

            }

            if (vertices.Count == 0) {
                log_error("Empty cloud.");
                return false;
            }


            return true;
        }

    }
}