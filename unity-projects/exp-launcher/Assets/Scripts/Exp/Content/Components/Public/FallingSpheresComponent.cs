
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
using System.Collections.Generic;
// unity
using UnityEngine;

namespace Ex { 

    public class FallingSphere{

        public GameObject sphere = null;

        public FallingSphere(Transform parent, Vector3 pos, float size, bool useCollider, float gravityForce) {

            sphere = GameObject.CreatePrimitive(PrimitiveType.Sphere);
            sphere.transform.SetParent(parent);
            sphere.transform.localPosition = pos;
            sphere.transform.localScale = new Vector3(size, size, size);
            sphere.layer = Layers.Default;
            var r = sphere.AddComponent<Rigidbody>();
            r.useGravity = true;

            var gravity = sphere.AddComponent<ConstantForce>();
            gravity.force = new Vector3(0.0f, -9.81f* gravityForce, 0.0f);

            sphere.GetComponent<SphereCollider>().enabled = useCollider;;            
            sphere.GetComponent<MeshRenderer>().material.color = Color.white;
            
        }

        public void clean() {
            GameObject.Destroy(sphere);
        }

    }

    public class FallingSpheresComponent : ExComponent{

        GameObject innerC = null;
        GameObject externC = null;
        GameObject parentSpheres = null;
        PhysicMaterial bounceMaterial = null;
        System.Random random = new System.Random();

        List<Tuple<FallingSphere, double>> spheres = new List<Tuple<FallingSphere, double>>();
        int spawnCounter = 0;

        protected override bool initialize() {

            bounceMaterial = Instantiate(Resources.Load<PhysicMaterial>("Materials/Components/FallingSpheres/bounce_ball")) as PhysicMaterial;

            innerC = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
            innerC.transform.SetParent(transform);
            innerC.transform.localScale = new Vector3(1f, 0.1f, 1f);
            innerC.layer = Layers.Default;
            innerC.name = "Inner spawn circle";
            innerC.GetComponent<MeshRenderer>().material.color = Color.red;
            innerC.SetActive(false);
            Destroy(innerC.GetComponent<CapsuleCollider>());

            externC = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
            externC.transform.SetParent(transform);
            externC.transform.localScale = new Vector3(1f, 0.1f, 1f);
            externC.layer = Layers.Default;
            externC.name = "Extern spawn circle";
            externC.GetComponent<MeshRenderer>().material.color = Color.green;
            externC.SetActive(false);
            Destroy(externC.GetComponent<CapsuleCollider>());

            parentSpheres = new GameObject("Parent spheres");
            parentSpheres.transform.SetParent(transform);
            parentSpheres.transform.localScale    = new Vector3(1f, 1f, 1f);
            parentSpheres.transform.localPosition = new Vector3(0f, 0f, 0f);
            parentSpheres.transform.localEulerAngles = new Vector3(0f, 0f, 0f);

            int max = initC.get<int>("max_number_spheres");
            for(int ii = 0; ii < max; ++ii) {
                spheres.Add(new Tuple<FallingSphere, double>(null, 0));
            }

            return true;
        }


        public override void update_from_current_config() {

            innerC.transform.localPosition = currentC.get_vector3("position");
            externC.transform.localPosition = currentC.get_vector3("position");// + new Vector3(0,0.1f, 0);
            float rayMin = currentC.get<float>("inner_circle_ray");
            float rayMax = currentC.get<float>("extern_circle_ray");
            innerC.transform.localScale = new Vector3(2 * rayMin, 0.2f, 2 * rayMin);
            externC.transform.localScale = new Vector3(2 * rayMax, 0.1f, 2 * rayMax);

            innerC.SetActive(currentC.get<bool>("display_spawner"));
            externC.SetActive(currentC.get<bool>("display_spawner"));
        }
        protected override void start_routine() {
            spawnCounter = 0;
        }
        protected override void stop_routine() {
            for (int ii = 0; ii < spheres.Count; ++ii) {
                if (spheres[ii].Item1 != null) {
                    spheres[ii].Item1.clean();
                    spheres[ii] = new Tuple<FallingSphere, double>(null, 0);
                }
            }
        }

        protected override void update() {
            
            var currTime = time().ellapsed_element_ms();

            // check if alive
            var maxDuration = currentC.get<int>("duration_time_sphere");
            for(int ii = 0; ii < spheres.Count; ++ii) {              
                if(spheres[ii].Item1 != null) {
                    if(currTime - spheres[ii].Item2 > maxDuration) { // destroy if life expired
                        spheres[ii].Item1.clean();
                        spheres[ii] = new Tuple<FallingSphere, double>(null, 0);
                    }
                }
            }

            var spawnTime = currentC.get<int>("spawn_time");
            double rayMin = currentC.get<float>("inner_circle_ray");
            double rayMax = currentC.get<float>("extern_circle_ray");        

            if (currTime / spawnTime > spawnCounter) {
           
                spawnCounter++;
                for (int ii = 0; ii < spheres.Count; ++ii) {
                    if (spheres[ii].Item1 == null) {

                        if(rayMax < rayMin) {
                            rayMax = rayMin;
                        }

                        double rAngle = random.NextDouble() *2 * Math.PI;
                        double rRay = (rayMax - rayMin) * Math.Sqrt(random.NextDouble()) + rayMin;
                        //double rRay = (rayMax - rayMin) * (random.NextDouble()) + rayMin;
                        float x = (float)(rRay * Math.Cos(rAngle));
                        float y = (float)(rRay * Math.Sin(rAngle));

                        Vector3 pos = currentC.get_vector3("position") + new Vector3(x,0f,y);
                        spheres[ii] = new Tuple<FallingSphere, double>(new FallingSphere(parentSpheres.transform, pos, 
                            currentC.get<float>("size_spheres"), currentC.get<bool>("collisions_enabled"), currentC.get<float>("gravity_extra_force")), currTime);
                        spheres[ii].Item1.sphere.GetComponent<SphereCollider>().sharedMaterial = bounceMaterial;
                        break;
                    }
                }
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void set_visibility(bool visibility) {

            bool display = currentC.get<bool>("display_spawner");
            innerC.SetActive(display && visibility);
            externC.SetActive(display && visibility);
            parentSpheres.SetActive(visibility);

            for (int ii = 0; ii < spheres.Count; ++ii) {
                if (spheres[ii].Item1 != null) {
                    spheres[ii].Item1.sphere.SetActive(visibility);
                }
            }
        }
    }
}