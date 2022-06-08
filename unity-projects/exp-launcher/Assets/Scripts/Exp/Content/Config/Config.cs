
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

    public static class ConfigUtility{

        public static Dictionary<string, TMPro.TextAlignmentOptions> textAlignment = null;

        public static void initialize() {

            if (textAlignment == null) {
                textAlignment = new Dictionary<string, TMPro.TextAlignmentOptions>();
                textAlignment["Middle left"] = TMPro.TextAlignmentOptions.BaselineLeft;
                textAlignment["Middle right"] = TMPro.TextAlignmentOptions.BaselineRight;
                textAlignment["Middle justified"] = TMPro.TextAlignmentOptions.BaselineJustified;
                textAlignment["Middle centered"] = TMPro.TextAlignmentOptions.BaselineGeoAligned;
                textAlignment["Bottom left"] = TMPro.TextAlignmentOptions.BottomLeft;
                textAlignment["Bottom right"] = TMPro.TextAlignmentOptions.BottomRight;
                textAlignment["Bottom justified"] = TMPro.TextAlignmentOptions.BottomJustified;
                textAlignment["Bottom centered"] = TMPro.TextAlignmentOptions.BottomGeoAligned;
                textAlignment["Top left"] = TMPro.TextAlignmentOptions.TopLeft;
                textAlignment["Top right"] = TMPro.TextAlignmentOptions.TopRight;
                textAlignment["Top justified"] = TMPro.TextAlignmentOptions.TopJustified;
                textAlignment["Top centered"] = TMPro.TextAlignmentOptions.TopGeoAligned;
            }
        }
    }

    public class Config : MonoBehaviour{

        public int key;
        public string keyStr;
        public Dictionary<string, Argument> args = null;


        public void log_error(string error, bool verbose = true) {
            if (verbose) {
                ExVR.Log().error(string.Format("From config {0}:{1} -> {2}", name, key, error));
            } else {
                ExVR.Log().error(error);
            }           
        }

        // contains

        public bool has(string argName) {
            return args.ContainsKey(argName);
        }

        #region get

        public Argument get(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0}  doesn't exist.", argName));
                return null;
            }
            return args[argName];
        }

        public T get<T>(string argName){

            Argument arg = get(argName);
            if(arg == null) {
                return default(T);
            }

            try {
                return (T)arg.value;
            } catch (System.InvalidCastException) {
                log_error(string.Format("Cannot cast argument {0} of type {1}:{2} of value {3}", argName, typeof(T).ToString(), args[argName].xml.Type, args[argName].xml.Value));
            }

            return default(T);
        }

        public void get<T>(string argName, ref T value) {
            value = get<T>(argName);
        }

        public void get(string argName, ref Color value) {
            if (has(argName)) {
                value = Converter.to_color((List<object>)args[argName].value);
            } else {
                value = Color.black;
                log_error(string.Format("Argument {0} of type Color doesn't exist.", argName));
            }
        }

        public void get(string argName, ref Vector2 value) {
            if (has(argName)) {
                value = Converter.to_vector2((List<object>)args[argName].value);
            } else {
                value = Vector2.zero;
                log_error(string.Format("Argument {0} of type Vector2 doesn't exist.", argName));
            }
        }

        public void get(string argName, ref Vector3 value) {
            if (has(argName)) {
                value = Converter.to_vector3((List<object>)args[argName].value);
            } else {
                value = Vector3.zero;
                log_error(string.Format("Argument {0} of type Vector3 doesn't exist.", argName));
            }
        }

        public void get(string argName, ref DecimalValue value) {
            if (has(argName)) {
                value = Converter.to_decimal(args[argName].value);
            } else {
                value = new DecimalValue(0);
                log_error(string.Format("Argument {0} of type DecimalValue doesn't exist.", argName));
            }
        }

        #endregion

        #region get_if_exists

        // get only if exists
        public void get_if_exists<T>(string argName, ref T value) {
            if (has(argName)) {
                value = get<T>(argName);
            }
        }

        public void get_if_exists(string argName, ref Color value) {
            if (has(argName)) {
                value = Converter.to_color((List<object>)args[argName].value);
            }
        }

        public void get_if_exists(string argName, ref Vector2 value) {
            if (has(argName)) {
                value = Converter.to_vector2((List<object>)args[argName].value);
            }
        }

        public void get_if_exists(string argName, ref Vector3 value) {
            if (has(argName)) {
                value = Converter.to_vector3((List<object>)args[argName].value);
            }
        }

        public void get_if_exists(string argName, ref DecimalValue value) {
            if (has(argName)) {
                value = Converter.to_decimal(args[argName].value);
            }
        }

        #endregion

        #region get_type

        public Color get_color(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Color doesn't exist.", argName));
                return new Color(0f, 0f, 0f);
            }
            return Converter.to_color((List<object>)args[argName].value);
        }

        public Vector2 get_vector2(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Vector2 doesn't exist.", argName));
                return new Vector2(0f, 0f);
            }
            return Converter.to_vector2((List<object>)args[argName].value);
        }

        public Vector3 get_vector3(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Vector3 doesn't exist.", argName));
                return new Vector3(0f, 0f, 0f);
            }
            return Converter.to_vector3((List<object>)args[argName].value);
        }

        public Vector3 get_euler_angles(string argName, Converter.AxisOrder order = Converter.AxisOrder.PitchYawRoll) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Vector3 doesn't exist.", argName));
                return new Vector3(0f, 0f, 0f);
            }
            return Converter.to_vector3((List<object>)args[argName].value, order);
        }


        public TransformValue get_transform_value(string argName, Converter.AxisOrder order = Converter.AxisOrder.PitchYawRoll) {
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Transform doesn't exist.", argName));
                return new TransformValue();
            }
            return Converter.to_transform_value(args[argName].value, order);
        }

        // others types
        public DecimalValue get_decimal(string argName) {
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type DecimalValue doesn't exist.", argName));
                return new DecimalValue();
            }
            return Converter.to_decimal(args[argName].value);
        }

        public List<T> get_list<T>(string argName) {
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type List doesn't exist.", argName));
                return new List<T>();
            }
            return Converter.to_list<T>(args[argName].value);
        }

        public AnimationCurve get_curve(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type AnimationCurve doesn't exist.", argName));
                return new AnimationCurve();
            }
            return Converter.to_animation_curve(get_list<float>(argName));
        }

        #endregion

        #region get_ex

        public T get_component<T>(string argName) where T : ExComponent {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} doesn't exist.", argName));
                return null;
            }

            var split = ((List<object>)args[argName].value);
            if (split.Count == 2) {
                string nameStr = (string)split[0];
                string idStr = (string)split[1];
                int key = Converter.to_int(idStr);
                if (key == -1) {
                    return null;
                }
                return (T)ExVR.Components().get_from_key(key);
            }
            return null;
        }

        public Tuple<Routine, Condition, ExComponent, ComponentConfig> get_component_config(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} doesn't exist.", argName));
                return null;
            }

            var split = ((List<object>)args[argName].value);
            if (split.Count == 6) {

                Condition condition = null;
                Routine routine = ExVR.Routines().get((string)split[2]);
                if (routine != null) {
                    condition = routine.get_condition_from_name((string)split[3]);
                }

                int componentKey = Converter.to_int((string)split[4]);
                int configKey = Converter.to_int((string)split[5]);
                if (componentKey == -1 || configKey == -1) {
                    log_error("Invalid component/config key.");
                    return null;
                }
                ExComponent component = ExVR.Components().get_from_key(componentKey);
                if (component != null) {
                    ComponentConfig config = component.get_config(configKey);
                    if (config != null) {
                        return new Tuple<Routine, Condition, ExComponent, ComponentConfig>(routine, condition, component, config);
                    } else {
                        log_error(string.Format("Cannot find config from key {0} in component {1}", config, component.name));
                    }
                } else {
                    log_error(string.Format("Cannot find component from key {0}", componentKey));
                }

            }
            return null;
        }
        public List<T> get_components_list<T>(string argName) where T : ExComponent {

            List<T> components = new List<T>();
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type ComponentsList doesn't exist.", argName));
                return components;
            }

            var split = ((List<object>)args[argName].value);
            foreach (var keyStr in split) {
                int key = Converter.to_int((string)keyStr);
                var component = ExVR.Components().get_from_key(key);
                if (component != null) {
                    components.Add((T)(component));
                }
            }
            return components;
        }

        #endregion

        #region get_resources_data

        public string get_resource_alias(string argName) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} doesn't exist.", argName));
                return "";
            }

            var split = ((List<object>)args[argName].value);
            if (split.Count == 2) {
                return (string)split[0];
            }
            return "";
        }


        public AudioResource get_resource_audio_data(string argName) {
            string audioAlias = get_resource_alias(argName);
            if (audioAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_audio_file_data(audioAlias);
        }

        public PlotResource get_resource_plot_data(string argName) {
            string plotAlias = get_resource_alias(argName);
            if (plotAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_plot_file_data(plotAlias);
        }

        public ImageResource get_resource_image_data(string argName) {
            string imageAlias = get_resource_alias(argName);
            if (imageAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_image_file_data(imageAlias);
        }

        public TextResource get_resource_text_data(string argName) {
            string textAlias = get_resource_alias(argName);
            if (textAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_text_file_data(textAlias);
        }

        public VideoResource get_resource_video_data(string argName) {
            string videoAlias = get_resource_alias(argName);
            if (videoAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_video_file_data(videoAlias);
        }

        public AssetBundleResource get_resource_asset_bundle_data(string argName) {
            string assetBundleAlias = get_resource_alias(argName);
            if (assetBundleAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_asset_bundle_file_data(assetBundleAlias);
        }

        public VolumetricVideoResource get_volumetric_video_data(string argName) {
            string volumetricVideoAlias = get_resource_alias(argName);
            if (volumetricVideoAlias.Length == 0) {
                return null;
            }
            return ExVR.Resources().get_volumetric_video_file_data(volumetricVideoAlias);
        }

        #endregion

        #region get_resource_sub_data

        public string get_resource_path(ResourcesManager.ResourceType type, string argName) {
            string alias = get_resource_alias(argName);
            if (alias.Length == 0) {
                return "";
            }
            return ExVR.Resources().get_resource_file_data(type, alias).path;
        }

        public Texture2D get_resource_image(string argName, bool returnDefaultIfNull = true) {

            var data = get_resource_image_data(argName);
            if (data == null) {
                if (returnDefaultIfNull) {
                    return ExVR.Resources().get_image_file_data("default_texture").texture;
                } else {
                    return null;
                }
            }
            return data.texture;
        }

        public string get_resource_text(string argName) {

            string txtAlias = get_resource_alias(argName);
            if (txtAlias.Length == 0) {
                return "text_not_found";
            }
            return ExVR.Resources().get_text_file_data(txtAlias).content;
        }

        #endregion

        #region get_resources_list

        public List<ImageResource> get_images_resources_list(string argName) {

            List<ImageResource> resources = new List<ImageResource>();
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type image doesn't exist.", argName));
                return resources;
            }

            foreach (var keyStr in ((List<object>)args[argName].value)) {
                int key = Converter.to_int((string)keyStr);
                var resourceData = ExVR.Resources().get_resource_file_data(ResourcesManager.ResourceType.Image, key);
                if (resourceData != null) {
                    resources.Add((ImageResource)(resourceData));
                }
            }
            return resources;
        }

        public List<AssetBundleResource> get_asset_bundles_resources_list(string argName) {

            List<AssetBundleResource> resources = new List<AssetBundleResource>();
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Asset bundle doesn't exist.", argName));
                return resources;
            }

            foreach (var keyStr in ((List<object>)args[argName].value)) {
                int key = Converter.to_int((string)keyStr);
                var resourceData = ExVR.Resources().get_resource_file_data(ResourcesManager.ResourceType.UnityAssetBundle, key);
                if (resourceData != null) {
                    resources.Add((AssetBundleResource)(resourceData));
                }
            }
            return resources;
        }

        public List<PlotResource> get_plots_resources_list(string argName) {

            List<PlotResource> resources = new List<PlotResource>();
            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type PlotResource doesn't exist.", argName));
                return resources;
            }

            foreach (var keyStr in ((List<object>)args[argName].value)) {
                int key = Converter.to_int((string)keyStr);
                var resourceData = ExVR.Resources().get_resource_file_data(ResourcesManager.ResourceType.Plot, key);
                if (resourceData != null) {
                    resources.Add((PlotResource)(resourceData));
                }
            }
            return resources;
        }

        #endregion

        #region set

        public void add<T>(string argName, T value) {

            if (has(argName)) {
                log_error(string.Format("Argument with name [{0}] of type [{1}] already exists.", argName, typeof(T).ToString()));
                return;
            }
            args[argName] = new Argument();
            args[argName].value = value;
            args[argName].type = typeof(T);
        }


        public void init_or_set<T>(string argName, T value) {
            if (has(argName)) {
                set(argName, value);
            } else {
                add(argName, value);
            }
        }

        public void set<T>(string argName, T value) {
            if (!has(argName)) {
                log_error(string.Format("Argument with name [{0}] of type [{1}] doesn't exist in config.", argName, typeof(T).ToString()));
                return;
            }
            args[argName].value = value;
        }


        public void set_vector2(string argName, Vector2 value) {
            set(argName, Converter.to_object_list(value));
        }

        public void set_vector3(string argName, Vector3 value) { 
            set(argName, Converter.to_object_list(value));
        }
        public void set_color(string argName, Color color) {
            set(argName, Converter.to_object_list(color));
        }

        public void set_transform(string argName, Transform transform, bool local = false) {
            set(argName, Converter.to_object_list(transform, local));
        }

        public void set_transform(string argName, TransformValue transform) {
            set(argName, Converter.to_object_list(transform));
        }

        public void set_transform(string argName, List<Vector3> transform) {
            set(argName, Converter.to_object_list(transform));
        }

        public void set_list<T>(string argName, List<T> values) {
            set(argName, Converter.to_object_list(values));
        }

        #endregion

        #region update

        public void update_transform(string argName, Transform transform, bool local = true, bool position = true, bool rotation = true, bool scale = true) {

            if (!has(argName)) {
                log_error(string.Format("Argument {0} of type Transform doesn't exist.", argName));
                return;
            }

            Apply.to_transform(args[argName].value, transform, local, position, rotation, scale);
        }

        public void update_text(string baseArgName, TMPro.TextMeshProUGUI text) {

            string argName = baseArgName + "_text_resource";
            if (has(argName)) { // check if contain text input
                string textAlias = get_resource_alias(argName);
                if (textAlias.Length == 0) {
                    text.SetText(get<string>(baseArgName + "_text"));
                } else {
                    text.SetText(ExVR.Resources().get_text_file_data(textAlias).content);
                }
                text.richText = get<bool>(baseArgName + "_rich_text");
            }

            // set font style
            var fontStyle = TMPro.FontStyles.Normal;
            if (get<bool>(baseArgName + "_bold")) {
                fontStyle |= TMPro.FontStyles.Bold;
            }
            if (get<bool>(baseArgName + "_italic")) {
                fontStyle |= TMPro.FontStyles.Italic;
            }
            if (get<bool>(baseArgName + "_highlight")) {
                fontStyle |= TMPro.FontStyles.Highlight;
            }
            if (get<bool>(baseArgName + "_lower_case")) {
                fontStyle |= TMPro.FontStyles.LowerCase;
            }
            if (get<bool>(baseArgName + "_upper_case")) {
                fontStyle |= TMPro.FontStyles.UpperCase;
            }
            if (get<bool>(baseArgName + "_under_line")) {
                fontStyle |= TMPro.FontStyles.Underline;
            }
            text.fontStyle = fontStyle;

            // set font size
            text.fontSize = get<float>(baseArgName + "_font_size");
            text.enableAutoSizing = get<bool>(baseArgName + "_auto_size");
            text.enableWordWrapping = get<bool>(baseArgName + "_wrap");

            // alignment
            text.alignment = ConfigUtility.textAlignment[get<string>(baseArgName + "_alignment")];

            // face
            var fc = get_color(baseArgName + "_face_color");
            if (fc == Color.white) { // bug if face color is white, it's not applied
                text.faceColor = new Color32(255, 255, 255, 254);
            } else {
                text.faceColor = fc;
            }            

            // ouline
            text.outlineColor = get_color(baseArgName + "_outline_color");
            text.outlineWidth = get<float>(baseArgName + "_outline_width");

            // spacing
            text.paragraphSpacing = get<float>(baseArgName + "_paragraph_spacing");
            text.lineSpacing = get<float>(baseArgName + "_line_spacing");
            text.wordSpacing = get<float>(baseArgName + "_word_spacing");
            text.characterSpacing = get<float>(baseArgName + "_character_spacing");

            text.UpdateFontAsset();

            // check if input font exists in list
            // if not do nothing
            // else CreateDynamicFontFromOSFont

            //TMP_FontAsset;
            //Font f = Resources.Load("Fonts/OTF/bimasakti") as Font;
            ////
            //// Résumé :
            ////     Get names of fonts installed on the machine.
            ////
            //// Retourne :
            ////     An array of the names of all fonts installed on the machine.
            //public static string[] GetOSInstalledFontNames();
            ////
            //// Résumé :
            ////     Gets the file paths of the fonts that are installed on the operating system.
            ////
            //// Retourne :
            ////     An array of the file paths of all fonts installed on the machine.
            //public static string[] GetPathsToOSFonts();

            //public static Font CreateDynamicFontFromOSFont(string fontname, int size);

            //Resources.GetBuiltinResourcke(typeof(Font), "Arial.ttf");
            // TrueType Fonts (.ttf files) and OpenType Fonts (.otf files).
            // font 
            //m_text.font = TMP_FontAsset.CreateFontAsset(new Font(currentC.get<string>("font_name")));
            //public bool isRightToLeftText { get; set; }
        }

        #endregion

        #region xml
        public bool update_from_xml(XML.Arg xmlArg) {

            // get the name from its id
            if (!has(xmlArg.Name)) {
                log_error(string.Format("Argument {0} doesn't exist.", xmlArg.Name));
                return false;
            }

            // convert xml to arg
            if (!args[xmlArg.Name].update_from_xml(xmlArg)) {
                log_error(string.Format("Cannot update argument {0} with value {1}.", xmlArg.Name, xmlArg.Value));
                return false;
            }

            return true;
        }

        public void init_from_xml(XML.Arg xmlArg) {

            args = new Dictionary<string, Argument>(1);
            Argument arg = new Argument();
            if (!arg.update_from_xml(xmlArg)) {
                log_error(string.Format("Cannot initialize argument {0} with value {1}.", xmlArg.Name, xmlArg.Value));
            }
            args[xmlArg.Name] = arg;
        }

        public void init_from_xml(List<XML.Arg> xmlArgs) {

            args = new Dictionary<string, Argument>(xmlArgs.Count);
            foreach (XML.Arg xmlArg in xmlArgs) {
                Argument arg = new Argument();
                if (!arg.update_from_xml(xmlArg)) {
                    log_error(string.Format("Cannot initialize argument {0} with value {1}.", xmlArg.Name,xmlArg.Value));
                }
                args[xmlArg.Name] = arg;
            }
        }

        #endregion
    }

    public class ComponentInitConfig : Config{

        public void initialize(XML.InitConfig initConfig) {
            name = "init";
            init_from_xml(initConfig.Arg);
            key = initConfig.Key;
            keyStr = Converter.to_string(key);
        }
    }

    public class ComponentConfig : Config{

        public void initialize(XML.Config config) {
            name = config.Name;
            init_from_xml(config.Arg);
            key = config.Key;
            keyStr = Converter.to_string(key);
        }
    }
}