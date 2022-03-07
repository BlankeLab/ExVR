
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
using System.Collections.Generic;
using System.IO;

// unity
using UnityEngine;

public class WalkerReader : MonoBehaviour{

    public List<List<Vector3>> pointsPerFrame = null;

    Color32[] colors = null;
    Color32[] background = null;

    Texture2D texture = null;
    Unity.Collections.NativeArray<Color32> nativeData;
    Unity.Collections.NativeArray<Color32> nativeBackground;


    public int height;
    public int width;
    public int widthWithBorders;
    public int heightWithBorders;
    public int border = 10;

    System.Diagnostics.Stopwatch watch;

    public int inputFrequency  = 30;
    public int outputFrequency = 90;


    public int nbOfLines = 0;

    float maxX = -10000;
    float minX = 10000;

    float maxY = -10000;
    float minY = 10000;

    float maxZ = -10000;
    float minZ = 10000;

    void Start(){

        //var content = File.ReadAllText("C:/Users/lance/Downloads/diameter/diameter/Data/walker-077-100Hz.txt").Split('\n');
        var content = File.ReadAllText("C:/Users/lance/Downloads/diameter/diameter/Data/walker-30Hz.txt").Split('\n');
        pointsPerFrame = new List<List<Vector3>>(content.Length);
        foreach (var line in content) {

            if(line.Length == 0) {
                continue;
            }

            var fullLine = line.Split(' ');
            var coordinates = fullLine[1].Split(',');
            var framePoints = new List<Vector3>(coordinates.Length/3);
            
            for(int ii = 0; ii < coordinates.Length/3; ++ii) {
                float x = Ex.Converter.to_float(coordinates[3 * ii]);
                float y = Ex.Converter.to_float(coordinates[3 * ii + 1]);
                float z = Ex.Converter.to_float(coordinates[3 * ii + 2]);

                if (x < minX) {
                    minX = x;
                }
                if (x > maxX) {
                    maxX = x;
                }

                if (y < minY) {
                    minY = y;
                }
                if (y > maxY) {
                    maxY = y;
                }

                if (z < minZ) {
                    minZ = z;
                }
                if (z > maxZ) {
                    maxZ = z;
                }

                framePoints.Add(new Vector3(x, y, z));
            }
            pointsPerFrame.Add(framePoints);            
        }

        float xDiff = maxX - minX;
        float yDiff = maxY - minY;
        float zDiff = maxZ - minZ;

        Debug.Log("Min: " + minX + " " + minY + " " + minZ);
        Debug.Log("Max: " + maxX + " " + maxY + " " + maxZ);

        width  = (int)Mathf.Ceil(yDiff); 
        height = (int)Mathf.Ceil(zDiff); 

        widthWithBorders  = width  + 2 * border;
        heightWithBorders = height + 2 * border;
       
        foreach (var frame in pointsPerFrame) {
            for (int ii = 0; ii < frame.Count; ++ii) {
                // we use only y and z
                float x = (frame[ii].y - minY) / yDiff;
                float y = (frame[ii].z - minZ) / zDiff;
                frame[ii] = new Vector2(x, y);                
            }
        }

        // resample

//        dx_adjusted = d * (MAX_X - MIN_X); % d percent of the entire width
//    dy_adjusted = d * (MAX_Y - MIN_Y); % d percent of the entire hight

//    for i = 1:size(mX, 2)
//%         % logic: (a - d) + rand * (a + d - (a - d));
//% mXs(:, i) = mX(:, i) - dx_adjusted + rand * 2 * dx_adjusted;
//% mYs(:, i) = mY(:, i) - dy_adjusted + rand * 2 * dy_adjusted;

//        % move the dot(a) randomly between[a - d / 2, a + d / 2]
//        mXs(:, i) = mX(:, i) - dx_adjusted / 2 + rand * dx_adjusted;
//        mYs(:, i) = mY(:, i) - dy_adjusted / 2 + rand * dy_adjusted;




        Debug.Log("width: " + width);
        Debug.Log("height: " + height);



        var black = new Color32(0, 0, 0, 255);
        var blue = new Color32(0, 0, 255, 255);
        var white = new Color32(255, 255, 255, 255);
        texture = new Texture2D(widthWithBorders, heightWithBorders);
        colors  = new Color32[widthWithBorders * heightWithBorders];
        background = new Color32[widthWithBorders * heightWithBorders];
        for (int ii = 0; ii < background.Length; ++ii) {
            background[ii] = black;
        }


        

        nativeBackground = new Unity.Collections.NativeArray<Color32>(background, Unity.Collections.Allocator.Persistent);
        //nativeData       = new Unity.Collections.NativeArray<Color32>(colors, Unity.Collections.Allocator.Temp);
        //nativeData = texture.GetRawTextureData<Color32>();

        clear_colors(black);
        draw_borders(blue);

        int ray = 10;


        // RGBA32 texture format data layout exactly matches Color32 struct

        watch = new System.Diagnostics.Stopwatch();
        
        Debug.Log("start: " + pointsPerFrame.Count);
        int id = 0;
        foreach (var frame in pointsPerFrame) {
            watch.Restart();


            //clear_colors(blue);

            nativeData = texture.GetRawTextureData<Color32>();
            //Debug.Log("GetRawTextureData: " + watch.ElapsedMilliseconds + " ms");
            var subArray = nativeData.GetSubArray(0, texture.width * texture.height);
            //Debug.Log("sub array: " + watch.ElapsedMilliseconds + " ms");

            subArray.CopyFrom(nativeBackground);
            



            //Debug.Log("clear: " + watch.ElapsedMilliseconds + " ms");



            //Debug.Log("points: " + frame.Count);

            int id2= 0;
            foreach (var point in frame) {

                //watch.Restart();
                draw_circle(
                    border + (int)(point.x * width),
                    border + (int)(point.y * height),
                    ray, new Color32((byte)id2, (byte)(254-id2), (byte)id2,255)
                );
                id2= (id2 + 10)%255;

            }
            //Debug.Log("draw_circles: " + watch.ElapsedMilliseconds + " ms");

            //watch.Restart();
            apply_colors();
            //Debug.Log("apply_colors: " + watch.ElapsedMilliseconds + " ms");

            SaveTextureAsPNG(texture, "C:/Users/lance/Downloads/diameter/diameter/Data/test_" + id + ".png");
            //Debug.Log("save: " + id);
            id++;
        }
        //Debug.Log("end " + watch.ElapsedMilliseconds + " ms");
    }

    void clear_colors(Color32 col) {


        //System.Array.Copy(new Color32[] { col }, colors, 1);
        //System.Array.Copy(background, 0, colors, 0, colors.Length);
        //System.Array.Copy(background, 0, colors, 0, colors.Length);


        //nativeData.CopyFrom(nativeBackground);

        //Debug.Log("GetRawTextureData");
        nativeData = texture.GetRawTextureData<Color32>();

        //Debug.Log("get sub array");
        var subArray = nativeData.GetSubArray(0, texture.width * texture.height);

        //Debug.Log("copy from");
        subArray.CopyFrom(nativeBackground);
        //Debug.Log("end copy from");





        //for (int ii = 0; ii < widthWithBorders * heightWithBorders; ++ii) {
        //    //data[ii] = col;
        //    colors[ii] = col;
        //    //colors[ii].r = col.r;
        //    //colors[ii].g = col.g;
        //    //colors[ii].b = col.b;
        //    //colors[ii].a = col.a;
        //}
    }

    void draw_borders(Color32 col) {
        nativeData = texture.GetRawTextureData<Color32>();

        for (int ii = 0; ii < widthWithBorders; ++ii) {
            for (int jj = 0; jj < heightWithBorders; ++jj) {                
                if (ii <= border || ii >= widthWithBorders - border || jj <= border || jj >= heightWithBorders - border) {
                    //colors[jj * widthWithBorders + ii] = col;
                    nativeData[jj * widthWithBorders + ii] = col;
                }
            }
        }
    }

    void apply_colors() {
        //texture.SetPixels32(colors);
        //texture.GetRawTextureData();
        texture.Apply();
    }

    void draw_circle(int x, int y, int r, Color32 col) {

        nativeData = texture.GetRawTextureData<Color32>();

        int grid = r;

        int xMin = x - grid;
        if(xMin < 0) {
            xMin = 0;
        }
        int yMin = y - grid;
        if(yMin < 0) {
            yMin = 0;
        }

        int xMax = x + grid;
        if (xMax > widthWithBorders) {
            xMax = widthWithBorders;
        }
        int yMax = y + grid;
        if (yMax > heightWithBorders) {
            yMax = heightWithBorders;
        }


        float rSquared = r * r;
        for (int ii = xMin; ii < xMax; ++ii) {

            int dX = (x - ii);
            int sX = dX * dX;
            for (int jj = yMin; jj < yMax; ++jj) {
                int dY = (y - jj);
                if (sX + dY * dY < rSquared) {
                    //colors[jj * widthWithBorders + ii] = col;
                    nativeData[jj * widthWithBorders + ii] = col;
                }
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public static void SaveTextureAsPNG(Texture2D _texture, string _fullPath) {
        byte[] _bytes = _texture.EncodeToPNG();
        System.IO.File.WriteAllBytes(_fullPath, _bytes);
        //Debug.Log(_bytes.Length / 1024 + "Kb was saved as: " + _fullPath);
    }
}

