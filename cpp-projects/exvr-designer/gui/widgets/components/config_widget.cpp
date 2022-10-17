
/***********************************************************************************
** exvr-designer                                                                  **
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

#include "config_widget.hpp"

// base
#include "utility/benchmark.hpp"

// local
#include "experiment/global_signals.hpp"

// # parameters
#include "config_parameters/fixation_cross_viewer_pw.hpp"
#include "config_parameters/python_script_pw.hpp"
#include "config_parameters/csharp_pw.hpp"
#include "config_parameters/duo_tracking_pw.hpp"
#include "config_parameters/asset_bundle_pw.hpp"
#include "config_parameters/slider_ui_pw.hpp"
#include "config_parameters/camera_target_pw.hpp"
#include "config_parameters/audio_source_pw.hpp"
#include "config_parameters/udp_reader_pw.hpp"
#include "config_parameters/udp_writer_pw.hpp"
#include "config_parameters/webcam_viewer_pw.hpp"
#include "config_parameters/joypad_pw.hpp"
#include "config_parameters/logger_pw.hpp"
#include "config_parameters/microphone_pw.hpp"
#include "config_parameters/scene_scaner_pw.hpp"
#include "config_parameters/mouse_pw.hpp"
#include "config_parameters/text_resource_pw.hpp"
#include "config_parameters/leap_motion_arms_display_pw.hpp"
#include "config_parameters/serial_port_reader_pw.hpp"
#include "config_parameters/serial_port_writer_pw.hpp"
#include "config_parameters/falling_spheres_pw.hpp"
#include "config_parameters/mirror_pw.hpp"
#include "config_parameters/text_viewer_pw.hpp"
#include "config_parameters/video_file_camera_pw.hpp"
#include "config_parameters/camera_pw.hpp"
#include "config_parameters/camera_trajectory_pw.hpp"
#include "config_parameters/camera_trajectory_file_pw.hpp"
#include "config_parameters/keyboard_pw.hpp"
#include "config_parameters/flashing_dot_pw.hpp"
#include "config_parameters/primitives_model_pw.hpp"
#include "config_parameters/mark_to_clean_pw.hpp"
#include "config_parameters/target_to_grab_pw.hpp"
#include "config_parameters/attach_object_to_hand_pw.hpp"
#include "config_parameters/biopac_pw.hpp"
#include "config_parameters/thera_trainer_tracking_pw.hpp"
#include "config_parameters/thera_trainer_platform_pw.hpp"
#include "config_parameters/video_file_pw.hpp"
#include "config_parameters/webcam_pw.hpp"
#include "config_parameters/image_viewer_pw.hpp"
#include "config_parameters/image_resource_pw.hpp"
#include "config_parameters/video_generator_pw.hpp"
#include "config_parameters/environment_pw.hpp"
#include "config_parameters/parallel_port_writer_pw.hpp"
#include "config_parameters/cloud_pw.hpp"
#include "config_parameters/scaner_video_pw.hpp"
#include "config_parameters/humanoid_controller_pw.hpp"
#include "config_parameters/k2_manager_pw.hpp"
#include "config_parameters/k2_body_tracking_pw.hpp"
#include "config_parameters/plot_resource_pw.hpp"
#include "config_parameters/qualisys_tracking_pw.hpp"
#include "config_parameters/mri_pw.hpp"
#include "config_parameters/fov_simulator_pw.hpp"
#include "config_parameters/sonceboz_sg_pw.hpp"
#include "config_parameters/multi_ab_pw.hpp"
#include "config_parameters/fop_robot_pw.hpp"
#include "config_parameters/blend_fade_viewer_pw.hpp"
#include "config_parameters/flag_pole_pw.hpp"
#include "config_parameters/humanoid_avatar_pw.hpp"
#include "config_parameters/fpp_camera_pw.hpp"
#include "config_parameters/fpp_avatar_camera_pw.hpp"
#include "config_parameters/tpp_avatar_camera_pw.hpp"
#include "config_parameters/volumetric_video_pw.hpp"
#include "config_parameters/buttons_ui_pw.hpp"
#include "config_parameters/k4_manager_pw.hpp"

using namespace tool::ex;



ConfigW::ConfigW(Config *config, Component *component, bool initConfig, std::map<QStringView,Arg> &args) :
      configKey(ConfigKey{config->key()}), componentKey(ComponentKey{component->key()}), name(config->name){

    Bench::start("[ConfigW generate widget]"sv, false);
        p = generate_parameters(component->type, initConfig);
        p->set_infos(componentKey, configKey, initConfig);
        p->insert_widgets();
        p->init_and_register_widgets();        
        p->create_connections();
        p->init_from_args(args);
        p->late_update_ui();
        p->end_stretch();        
    Bench::stop();

    setLayout(new QHBoxLayout());
    layout()->addWidget(p);
    p->setVisible(true);
    p->update_from_resources();
    p->update_from_components();
}

void ConfigW::update_from_config(Config *config){

    name = config->name;

    Bench::start("[ConfigW::update_from_resources]"sv);
        p->update_from_resources();
    Bench::stop();

    Bench::start("[ConfigW::update_from_components]"sv);
        p->update_from_components();
    Bench::stop();
}

void ConfigW::update_with_info(QStringView id, QStringView value){
    p->update_with_info(id, value);
}

void ConfigW::reset_args(){
    p->reset_args();
}

ConfigParametersW *ConfigW::generate_parameters(Component::Type type, bool initConfig){

    using CT = Component::Type;
    using CC = Component::Category;

    switch (type) {
    // ############################# Avatar
    case CT::Humanoid_avatar:
        return gen_params_w<HumanoidAvatarInitConfigParametersW,HumanoidAvatarConfigParametersW>(initConfig);
    case CT::Humanoid_controller:
        return gen_params_w<HumanoidControllerInitConfigParametersW,HumanoidControllerConfigParametersW>(initConfig);
    // ############################# Audio
    case CT::AudioSource:
        return gen_params_w<AudioSourceInitConfigParametersW,AudioSourceConfigParametersW>(initConfig);
    case CT::Microphone:
        return gen_params_w<MicrophoneInitConfigParametersW,MicrophoneConfigParametersW>(initConfig);
    // ############################# Camera
    case CT::Camera:
        return gen_params_w<CameraInitConfigParametersW,CameraConfigParametersW>(initConfig);
    case CT::Camera_trajectory:
        return gen_params_w<CameraTrajectoryInitConfigParametersW,CameraTrajectoryConfigParametersW>(initConfig);
    case CT::Camera_trajectory_file:
        return gen_params_w<CameraTrajectoryFileInitConfigParametersW,CameraTrajectoryFileConfigParametersW>(initConfig);
    case CT::Camera_target:
        return gen_params_w<CameraTargetInitConfigParametersW,CameraTargetConfigParametersW>(initConfig);
    case CT::FPP_avatar_camera:
        return gen_params_w<FPPAvatarCameraInitConfigParametersW,FPPAvatarCameraConfigParametersW>(initConfig);
    case CT::FPP_camera:
        return gen_params_w<FPPCameraInitConfigParametersW,FPPCameraConfigParametersW>(initConfig);
    case CT::TPP_avatar_camera:
        return gen_params_w<TPPAvatarCameraInitConfigParametersW,TPPAvatarCameraConfigParametersW>(initConfig);
    // ############################# Cloud
    case CT::Cloud:
        return gen_params_w<CloudInitConfigParametersW,CloudConfigParametersW>(initConfig);
    case CT::Scaner_video:
        return gen_params_w<ScanerVideoInitConfigParametersW,ScanerVideoConfigParametersW>(initConfig);
    // ############################# Environment
    case CT::Post_process:
        return gen_params_w<PostProcessInitConfigParametersW,PostProcessConfigParametersW>(initConfig);
    case CT::Sky:
        return gen_params_w<SkyInitConfigParametersW,SkyConfigParametersW>(initConfig);
    // ############################# Flow
    case CT::Config:
        return gen_params_w<ConfigParametersW,ConfigParametersW>(initConfig);
    // ############################# Input
    case CT::Joypad:
        return gen_params_w<JoypadInitConfigParametersW,JoypadConfigParametersW>(initConfig);
    case CT::Keyboard:
        return gen_params_w<KeyboardInitConfigParametersW,KeyboardConfigParametersW>(initConfig);
    case CT::Mouse:
        return gen_params_w<MouseInitConfigParametersW,MouseConfigParametersW>(initConfig);
    // ############################# Interaction
    case CT::Flag_pole:
        return gen_params_w<FlagPoleInitConfigParametersW,FlagPoleConfigParametersW>(initConfig);
    case CT::Mark_to_clean:
        return gen_params_w<MarkToCleanInitConfigParametersW,MarkToCleanConfigParametersW>(initConfig);
    case CT::Target_to_grab:
        return gen_params_w<TargetToGrabInitConfigParametersW,TargetToGrabConfigParametersW>(initConfig);
    // ############################# Model
    case CT::Cube:
        return gen_params_w<CubeInitConfigParametersW,ModelConfigParametersW>(initConfig);
    case CT::Cylinder:
        return gen_params_w<CylinderInitConfigParametersW,ModelConfigParametersW>(initConfig);
    case CT::Landmark:
        return gen_params_w<LandmarkInitConfigParametersW,LandmarkConfigParametersW>(initConfig);
    case CT::Lines:
        return gen_params_w<LinesInitConfigParametersW,LinesConfigParametersW>(initConfig);
    case CT::Plane:
        return gen_params_w<PlaneInitConfigParametersW,ModelConfigParametersW>(initConfig);
    case CT::Sphere:
        return gen_params_w<SphereInitConfigParametersW,ModelConfigParametersW>(initConfig);
    case CT::Torus:
        return gen_params_w<TorusInitConfigParametersW,ModelConfigParametersW>(initConfig);
    // ############################# Network
    case CT::Parallel_port_writer:
        return gen_params_w<ParallelPortWriterInitConfigParametersW,ParallelPortWriterConfigParametersW>(initConfig);
    case CT::Serial_port_reader:
        return gen_params_w<SerialPortReaderInitParameterW,SerialPortReaderConfigParametersW>(initConfig);
    case CT::Serial_port_writer:
        return gen_params_w<SerialPortWriterInitParameterW,SerialPortWriterConfigParametersW>(initConfig);
    case CT::Udp_reader:
        return gen_params_w<UdpReaderInitConfigParametersW,UdpReaderConfigParametersW>(initConfig);
    case CT::Udp_writer:
        return gen_params_w<UdpWriterInitConfigParametersW,UdpWriterConfigParametersW>(initConfig);
    // ############################# Output
    case CT::Global_logger:
        return gen_params_w<GlobalLoggerInitConfigParametersW,GlobalLoggerConfigParametersW>(initConfig);
    case CT::Logger:
        return gen_params_w<LoggerInitConfigParametersW,LoggerConfigParametersW>(initConfig);
    case CT::LoggerColumns:
        return gen_params_w<LoggerColumnsInitConfigParametersW,LoggerColumnsConfigParametersW>(initConfig);
    case CT::LoggerCondition:
        return gen_params_w<LoggerConditionInitConfigParametersW,LoggerConditionConfigParametersW>(initConfig);
    case CT::LoggerExperiment:
        return gen_params_w<LoggerExperimentInitConfigParametersW,LoggerExperimentConfigParametersW>(initConfig);
    // ############################# Resource
    case CT::Image_resource:
        return gen_params_w<ImageResourceInitConfigParametersW,ImageResourceConfigParametersW>(initConfig);
    case CT::Plot_resource:
        return gen_params_w<PlotResourceInitConfigParametersW,PlotResourceConfigParametersW>(initConfig);
    case CT::Text_resource:
        return gen_params_w<TextResourceInitConfigParametersW,TextResourceConfigParametersW>(initConfig);
    case CT::Volumetric_video:
        return gen_params_w<VolumetricVideoInitConfigParametersW,VolumetricVideoConfigParametersW>(initConfig);
    // ############################# Scene
    case CT::Falling_spheres:
        return gen_params_w<FallingSpheresInitConfigParametersW,FallingSpheresConfigParametersW>(initConfig);
    case CT::Flashing_dot:
        return gen_params_w<FlashingDotInitConfigParametersW,FlashingDotConfigParametersW>(initConfig);
    case CT::Mirror:
        return gen_params_w<MirrorInitConfigParametersW,MirrorConfigParametersW>(initConfig);
    case CT::MRI:
        return gen_params_w<MriInitConfigParametersW,MriConfigParametersW>(initConfig);
    case CT::Multi_AB:
        return gen_params_w<MultiABInitConfigParametersW,MultiABConfigParametersW>(initConfig);
    case CT::Unity_asset_bundle:
        return gen_params_w<AssetBundleInitConfigParametersW,AssetBundleConfigParametersW>(initConfig);
    // ############################# Script
    case CT::CSharp_function:
        return gen_params_w<CSharpFunctionInitConfigParametersW,CSharpFunctionConfigParametersW>(initConfig);
    case CT::CSharp_script:
        return gen_params_w<CSharpScriptInitConfigParametersW,CSharpScriptConfigParametersW>(initConfig);
    case CT::Python_script:
        return gen_params_w<PythonScriptInitConfigParametersW,PythonScriptConfigParametersW>(initConfig);
    // ############################# Tracking
    case CT::Attach_object_to_hand:
        return gen_params_w<AttachObjectToHandInitConfigParametersW,AttachObjectToHandConfigParametersW>(initConfig);
    case CT::Biopac:
        return gen_params_w<BiopacInitConfigParametersW,BiopacConfigParametersW>(initConfig);
    case CT::Fop_robot:
        return gen_params_w<FopRobotInitConfigParametersW,FopRobotConfigParametersW>(initConfig);
    case CT::Qualisys:
        return gen_params_w<QualisysTrackingInitConfigParametersW,QualisysTrackingConfigParametersW>(initConfig);
    case CT::K2_manager:
        return gen_params_w<K2ManagerInitConfigParametersW,K2ManagerConfigParametersW>(initConfig);
    case CT::K2_body_tracking:
        return gen_params_w<K2BodyTrackingInitConfigParametersW,K2BodyTrackingConfigParametersW>(initConfig);
    case CT::K4_manager:
        return gen_params_w<K4ManagerInitConfigParametersW,K4ManagerConfigParametersW>(initConfig);
    case CT::Leap_motion:
        return gen_params_w<LeapMotionInitConfigParametersW,LeapMotionConfigParametersW>(initConfig);
    case CT::Leap_motion_arms_display:
        return gen_params_w<LeapMotionArmsDisplayInitConfigParametersW,LeapMotionArmsDisplayConfigParametersW>(initConfig);
    case CT::Scene_scaner:
        return gen_params_w<SceneScanerInitConfigParametersW,SceneScanerConfigParametersW>(initConfig);
    case CT::Sonceboz_SG:
        return gen_params_w<SoncebozSgInitConfigParametersW,SoncebozSgConfigParametersW>(initConfig);
    case CT::Thera_trainer_tracking:
        return gen_params_w<TheraTrainerTrackingInitConfigParametersW,TheraTrainerTrackingConfigParametersW>(initConfig);
    case CT::Thera_trainer_platform:
        return gen_params_w<TheraTrainerPlatformInitConfigParametersW,TheraTrainerPlatformConfigParametersW>(initConfig);
    // ############################# UI
    case CT::Buttons_ui:
        return gen_params_w<ButtonsUiInitConfigParametersW,ButtonsUiConfigParametersW>(initConfig);
    case CT::Slider_ui:
        return gen_params_w<SliderUiInitConfigParametersW,SliderUiConfigParametersW>(initConfig);
    // ############################# Video
    case CT::Video_file:
        return gen_params_w<VideoFileInitConfigParametersW,VideoFileConfigParametersW>(initConfig);
    case CT::Video_saver:
        return gen_params_w<VideoGeneratorInitConfigParametersW,VideoGeneratorConfigParametersW>(initConfig);
    case CT::Webcam:
        return gen_params_w<WebcamInitConfigParametersW,WebcamConfigParametersW>(initConfig);
    // ############################# Viewer
    case CT::Blend_fade_viewer:
        return gen_params_w<BlendFadeViewerInitConfigParametersW,BlendFadeViewerConfigParametersW>(initConfig);
    case CT::Fixation_cross_viewer:
        return gen_params_w<FixationCrossViewerInitConfigParametersW,FixationCrossViewerConfigParametersW>(initConfig);
    case CT::Fov_simulator_viewer:
        return gen_params_w<FovSimulatorInitConfigParametersW,FovSimulatorConfigParametersW>(initConfig);
    case CT::Image_viewer:
        return gen_params_w<ImageViewerInitConfigParametersW,ImageViewerConfigParametersW>(initConfig);
    case CT::Text_viewer:
        return gen_params_w<TextViewerInitConfigParametersW,TextViewerConfigParametersW>(initConfig);
    case CT::Video_file_camera_viewer:
        return gen_params_w<VideoFileCameraInitConfigParametersW,VideoFileCameraConfigParametersW>(initConfig);
    case CT::Webcam_viewer:
        return gen_params_w<WebcamViewerInitConfigParametersW,WebcamViewerConfigParametersW>(initConfig);
    default:
        return new ConfigParametersW();
    }
}

#include "moc_config_widget.cpp"
