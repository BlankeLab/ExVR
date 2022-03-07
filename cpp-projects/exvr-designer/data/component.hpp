
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

#pragma once

// Qt
#include <QStringList>

// base
#include "utility/vector.hpp"

// qt-utility
#include "qt_str.hpp"
#include "data/id_key.hpp"

// local
#include "forward.hpp"
#include "config.hpp"
#include "connection_node.hpp"


namespace tool::ex {

using namespace std::literals::string_view_literals;

struct Component;
using ComponentUP = std::unique_ptr<Component>;


struct Component {


    enum class Category : int {
        Audio, Avatar, Camera, Cloud, Environment, Flow,
        Input, Interaction, Model, Network, Output, Resource, Scene, Script, Tracking, UI, Video, Viewer,
        SizeEnum};
    using C  = Category;

    using Name = std::string_view;
    using TCategory = std::tuple<Category, Name>;
    static constexpr TupleArray<Category::SizeEnum,TCategory> categories ={{
        TCategory
        {Category::Audio,       "Audio"sv         },
        {Category::Avatar,      "Avatar"sv        },
        {Category::Camera,      "Camera"sv        },
        {Category::Cloud,       "Cloud"sv         },
        {Category::Environment, "Environment"sv   },
        {Category::Flow,        "Flow"sv          },
        {Category::Input,       "Input"sv         },
        {Category::Interaction, "Interaction"sv   },
        {Category::Model,       "Model"sv         },
        {Category::Network,     "Network"sv       },
        {Category::Output,      "Output"sv        },
        {Category::Resource,    "Resource"sv      },
        {Category::Scene,       "Scene"sv         },
        {Category::Script,      "Script"sv        },
        {Category::Tracking,    "Tracking"sv      },
        {Category::UI,          "UI"sv            },
        {Category::Video,       "Video"sv         },
        {Category::Viewer,      "Viewer"sv        },
    }};

    [[maybe_unused]] static Name to_string(Category c) {
        return categories.at<0,1>(c);
    }
    [[maybe_unused]] static std::optional<Category> get_category(const Name name) {
        return categories.optional_at<1,0>(name);
    }

    static auto all_categories() {
        return categories.tuple_column<0>();
    }

    static auto all_categories_name(){
        return categories.tuple_column<1>();
    }

    enum class Type : int {
        /** A */ AudioListener, AudioSource, Attach_object_to_hand,
        /** B */ Biopac, Blend_fade_viewer, Buttons_ui,
        /** C */ Camera, Camera_target, Camera_trajectory, Camera_trajectory_file, Cloud, Config, CSharp_function, CSharp_script, Cube, Cylinder,
        /** F */ Falling_spheres, Fixation_cross_viewer, Flag_pole, Flashing_dot, Fop_robot, Fov_simulator_viewer, FPP_avatar_camera, FPP_camera,
        /** H */ Humanoid_avatar, Humanoid_controller,
        /** I */ Image_resource, Image_viewer,
        /** J */ Joypad,
        /** K */ Keyboard, Kinect_manager, Kinect_body_tracking,
        /** L */ Landmark, Leap_motion, Leap_motion_arms_display, Leap_motion_tracking, Lines, Logger, LoggerColumns, LoggerCondition,
        /** M */ Mark_to_clean, Microphone, Mirror, Mouse, MRI, Multi_AB,
        /** P */ Parallel_port_writer, Plot_resource, Python_script,
        /** Q */ Qualisys,
        /** S */ Scaner_video, Scene_scaner, Serial_port_reader, Serial_port_writer, Sky, Slider_ui, Sonceboz_SG, Sphere,
        /** T */ Target_to_grab, Text_resource, Text_viewer, Thera_trainer_tracking, Thera_trainer_platform, Torus, TPP_avatar_camera,
        /** U */ Unity_asset_bundle, Udp_reader, Udp_writer,
        /** V */ Video_file, Video_file_camera_viewer, Video_saver, Vive_pro_eye_tracking, Volumetric_video,
        /** W */ Webcam, Webcam_viewer,
        SizeEnum
    };
    using T = Type;

    enum class TimelineO : int {
        Both, Visibility, Update, None,
        B = Both, V = Visibility, U = Update, N = None,
        SizeEnum
    };
    using TO = TimelineO;

    enum class ConfigO : int {
        Both, Initalization, Condition, None,
        B = Both, I = Initalization, C = Condition, N = None,
        SizeEnum
    };
    using CO = ConfigO;

    enum class Restricted : int {
        OpenSource, ClosedSource, LNCO
    };
    using R = Restricted;

    enum class State : int {
        Sta, Exp, Leg
    };    
    using S = State;


    using SV            = std::string_view;
    using TypeStr       = SV;
    using FullStr       = SV;
    using UnityStr      = SV;
    using IconStr       = SV;
    using Unicity       = bool;
    using Experimental  = bool;

    static constexpr size_t ColType         = 0;
    static constexpr size_t ColCategory     = 1;
    static constexpr size_t ColTimelineO    = 2;
    static constexpr size_t ColConfigO      = 3;
    static constexpr size_t ColUnicity      = 4;
    static constexpr size_t ColRestricted   = 5;
    static constexpr size_t ColState        = 6;
    static constexpr size_t ColTypeStr      = 7;
    static constexpr size_t ColFullStr      = 8;
    static constexpr size_t ColUnityStr     = 9;
    static constexpr size_t ColIconStr      = 10;

    using TComponent = std::tuple<
        Type,                         Category,       TimelineO, ConfigO, Unicity, Restricted,      State, TypeStr, FullStr, UnityStr, IconStr>;
    static constexpr TupleArray<T::SizeEnum, TComponent> components ={{
        TComponent
        // Audio
        {T::AudioListener,            C::Audio,       TO::U,     CO::B,   true,    R::OpenSource,   S::Exp, "AudioListener"sv, "Audio listener"sv, "AudioListener"sv, ":/icons/Sound"sv},
        {T::AudioSource,              C::Audio,       TO::B,     CO::B,   false,   R::OpenSource,   S::Sta, "AudioSource"sv, "Audio source"sv, "AudioSource"sv, ":/icons/Sound"sv},
        {T::Microphone,               C::Audio,       TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Microphone"sv, "Microphone"sv, "Microphone"sv, ":/icons/Micro"sv},
        // Avatar
        {T::Humanoid_avatar,          C::Avatar,      TO::B,     CO::B,   false,   R::ClosedSource, S::Sta, "Humanoid_avatar"sv, "Humanoid avatar"sv, "HumanoidAvatar"sv, ":/icons/Avatar"sv},
        {T::Humanoid_controller,      C::Avatar,      TO::B,     CO::B,   false,   R::ClosedSource, S::Leg, "Humanoid_controller"sv, "Humanoid controller"sv, "HumanoidController"sv, ":/icons/Avatar"sv},
        // Camera
        {T::Camera,                   C::Camera,      TO::U,     CO::B,   true,    R::OpenSource,   S::Sta, "Camera"sv, "Camera"sv, "Camera"sv, ":/icons/Camera"sv},
        {T::Camera_target,            C::Camera,      TO::U,     CO::C,   false,   R::OpenSource,   S::Sta, "Camera_target"sv, "Camera target"sv, "CameraTarget"sv, ":/icons/Camera"sv},
        {T::Camera_trajectory,        C::Camera,      TO::B,     CO::C,   false,   R::OpenSource,   S::Leg, "Camera_trajectory"sv, "Camera trajectory"sv, "CameraTrajectory"sv, ":/icons/Camera"sv},
        {T::Camera_trajectory_file,   C::Camera,      TO::B,     CO::C,   false,   R::OpenSource,   S::Leg, "Camera_trajectory_file"sv, "Camera trajectory file"sv, "CameraTrajectoryFile"sv, ":/icons/Camera"sv},
        {T::FPP_avatar_camera,        C::Camera,      TO::U,     CO::B,   false,   R::ClosedSource, S::Sta, "FPP_avatar_camera"sv, "First person perspective avatar camera"sv, "FPPAvatarCamera"sv, ":/icons/Camera"sv},
        {T::TPP_avatar_camera,        C::Camera,      TO::U,     CO::B,   false,   R::ClosedSource, S::Sta, "TPP_avatar_camera"sv, "Third persond perspective avatar camera"sv, "TPPAvatarCamera"sv, ":/icons/Camera"sv},
        {T::FPP_camera,               C::Camera,      TO::U,     CO::B,   false,   R::OpenSource,   S::Sta, "FPP_camera"sv, "First person perspective camera"sv, "FPPCamera"sv, ":/icons/Camera"sv},
        // Cloud
        {T::Cloud,                    C::Cloud,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Cloud"sv, "Cloud"sv, "Cloud"sv, ":/icons/Cloud"sv},
        {T::Scaner_video,             C::Cloud,       TO::B,     CO::B,   false,   R::LNCO,         S::Exp, "Scaner_video"sv, "Scaner video"sv, "ScanerVideo"sv, ":/icons/Video_cloud"sv},
        // Environment
        {T::Sky,                      C::Environment, TO::N,     CO::C,   true,    R::OpenSource,   S::Sta, "Sky"sv, "Sky"sv, "Sky"sv, ":/icons/Sky"sv},
        // Flow
        {T::Config,                   C::Flow,        TO::N,     CO::C,   false,   R::OpenSource,   S::Sta, "Config"sv, "Config"sv, "Config"sv, ":/icons/Sky"sv},
        // Input
        {T::Joypad,                   C::Input,       TO::U,     CO::I,   true,    R::OpenSource,   S::Sta, "Joypad"sv, "Joypad"sv, "Joypad"sv,":/icons/Joypad"sv},
        {T::Keyboard,                 C::Input,       TO::U,     CO::I,   true,    R::OpenSource,   S::Sta, "Keyboard"sv, "Keyboard"sv, "Keyboard"sv, ":/icons/Keyboard"sv},
        {T::Mouse,                    C::Input,       TO::U,     CO::I,   true,    R::OpenSource,   S::Sta, "Mouse"sv, "Mouse"sv, "Mouse"sv, ":/icons/Mouse"sv},
        // Interaction
        {T::Flag_pole,                C::Interaction, TO::B,     CO::B,   false,   R::OpenSource,   S::Sta, "Flag_pole"sv, "Flag pole"sv, "FlagPole"sv, ":/icons/Flag_pole"sv},
        {T::Mark_to_clean,            C::Interaction, TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Mark_to_clean"sv, "Mark to clean"sv, "MarkToClean"sv, ":/icons/Mark_to_clean"sv},
        {T::Target_to_grab,           C::Interaction, TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Target_to_grab"sv, "Target to grab"sv, "TargetToGrab"sv, ":/icons/Grab_target"sv},
        // Model
        {T::Cube,                     C::Model,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Cube"sv, "Cube"sv, "Cube"sv,":/icons/Cube"sv},
        {T::Cylinder,                 C::Model,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Cylinder"sv, "Cylinder"sv, "Cylinder"sv,":/icons/Cylinder"sv},
        {T::Landmark,                 C::Model,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Landmark"sv, "Landmark"sv, "Landmark"sv,":/icons/Landmark"sv},
        {T::Lines,                    C::Model,       TO::V,     CO::C,   false,   R::OpenSource,   S::Exp, "Lines"sv, "Lines"sv, "Lines"sv,":/icons/Line"sv},
        {T::Sphere,                   C::Model,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Sphere"sv, "Sphere"sv, "Sphere"sv,":/icons/Sphere"sv},
        {T::Torus,                    C::Model,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Torus"sv, "Torus"sv, "Torus"sv,":/icons/Torus"sv},
        // Network
        {T::Parallel_port_writer,     C::Network,     TO::N,     CO::B,   false,   R::OpenSource,   S::Sta, "Parallel_port_writer"sv, "Parallel port writer"sv, "ParallelPortWriter"sv, ":/icons/USB"sv},
        {T::Serial_port_reader,       C::Network,     TO::U,     CO::B,   false,   R::OpenSource,   S::Sta, "Serial_port_reader"sv, "Serial port reader"sv, "SerialPortReader"sv, ":/icons/USB"sv},
        {T::Serial_port_writer,       C::Network,     TO::U,     CO::B,   false,   R::OpenSource,   S::Sta, "Serial_port_writer"sv, "Serial port writer"sv, "SerialPortWriter"sv, ":/icons/USB"sv},
        {T::Udp_reader,               C::Network,     TO::U,     CO::I,   false,   R::OpenSource,   S::Sta, "Udp_reader"sv, "UDP reader"sv, "UdpReader"sv, ":/icons/UDP"sv},
        {T::Udp_writer,               C::Network,     TO::U,     CO::I,   false,   R::OpenSource,   S::Sta, "Udp_writer"sv, "UDP writer"sv, "UdpWriter"sv, ":/icons/UDP"sv},
        // Output
        {T::Logger,                   C::Output,      TO::N,     CO::I,   false,   R::OpenSource,   S::Sta, "Logger"sv, "Logger"sv, "Logger"sv, ":/icons/Logger"sv},
        {T::LoggerColumns,            C::Output,      TO::N,     CO::I,   false,   R::OpenSource,   S::Sta, "Logger_columns"sv, "Logger columns"sv, "LoggerColumns"sv, ":/icons/Logger"sv},
        {T::LoggerCondition,          C::Output,      TO::N,     CO::I,   false,   R::OpenSource,   S::Sta, "Logger_condition"sv, "Logger condition"sv, "LoggerCondition"sv, ":/icons/Logger"sv},
        // Resource
        {T::Image_resource,           C::Resource,    TO::N,     CO::B,   false,   R::OpenSource,   S::Sta, "Image_resource"sv, "Image resource"sv, "ImageResource"sv, ":/icons/Image"sv},
        {T::Plot_resource,            C::Resource,    TO::N,     CO::C,   false,   R::OpenSource,   S::Sta, "Plot_resource"sv,  "Plot resource"sv, "PlotResource"sv, ":/icons/Plot"sv},
        {T::Text_resource,            C::Resource,    TO::N,     CO::C,   false,   R::OpenSource,   S::Sta, "Text_resource"sv,  "Text resource"sv, "TextResource"sv, ":/icons/Text"sv},        
        // Scene
        {T::Falling_spheres,          C::Scene,       TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Falling_spheres"sv, "Falling spheres"sv, "FallingSpheres"sv, ":/icons/Falling_spheres"sv},
        {T::Flashing_dot,             C::Scene,       TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Flashing_dot"sv, "Flashing dot"sv, "FlashingDot"sv,":/icons/Dot"sv},
        {T::Mirror,                   C::Scene,       TO::V,     CO::C,   false,   R::OpenSource,   S::Leg, "Mirror"sv, "Mirror"sv, "Mirror"sv, ":/icons/Mirror"sv},
        {T::MRI,                      C::Scene,       TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "MRI"sv, "MRI"sv, "MRI"sv, ":/icons/MRI"sv},
        {T::Multi_AB,                 C::Scene,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Multi_AB"sv, "Multi assets bundles"sv, "MultiAB"sv, ":/icons/Unity_scene_bundle"sv},
        {T::Unity_asset_bundle,       C::Scene,       TO::V,     CO::B,   false,   R::OpenSource,   S::Sta, "Unity_asset_bundle"sv, "Unity asset bundle"sv, "AssetBundle"sv, ":/icons/Unity_scene_bundle"sv},
        // Script
        {T::CSharp_function,          C::Script,      TO::N,     CO::B,   false,   R::OpenSource,   S::Sta, "CSharp_function"sv, "CSharp function"sv, "CSharpFunction"sv, ":/icons/CSharp"sv},
        {T::CSharp_script,            C::Script,      TO::B,     CO::B,   false,   R::OpenSource,   S::Sta, "CSharp_script"sv, "CSharp script"sv, "CSharpScript"sv, ":/icons/CSharp"sv},
        {T::Python_script,            C::Script,      TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Python_script"sv, "Python script"sv, "PythonScript"sv, ":/icons/Python_script"sv},
        // Tracking
        {T::Attach_object_to_hand,    C::Tracking,    TO::B,     CO::B,   true,    R::OpenSource,   S::Exp, "Attach_object_to_hand"sv, "Attach object to hand"sv, "AttachObjectToHand"sv, ":/icons/Sponge"sv},
        {T::Biopac,                   C::Tracking,    TO::U,     CO::I,   true,    R::OpenSource,   S::Sta, "Biopac"sv, "Biopac device"sv, "Biopac"sv, ":/icons/Physio"sv},
        {T::Fop_robot,                C::Tracking,    TO::U,     CO::B,   true,    R::LNCO,         S::Sta, "Fop_robot"sv, "FOP robot"sv, "FOPRobot"sv,":/icons/Fop_robot"sv},
        {T::Kinect_manager,           C::Tracking,    TO::U,     CO::B,   true,    R::LNCO,         S::Sta, "Kinect_manager"sv, "Kinect manager"sv, "KinectManager"sv,":/icons/Kinect"sv},
        {T::Kinect_body_tracking,     C::Tracking,    TO::B,     CO::B,   true,    R::LNCO,         S::Exp, "Kinect_body_tracking"sv, "Kinect body tracking"sv, "KinectBodyTracking"sv,":/icons/Kinect"sv},
        {T::Leap_motion,              C::Tracking,    TO::U,     CO::N,   true,    R::OpenSource,   S::Leg, "Leap_motion"sv, "LeapMotion"sv, "LeapMotion"sv, ":/icons/Hand"sv},
        {T::Leap_motion_arms_display, C::Tracking,    TO::V,     CO::B,   true,    R::ClosedSource, S::Leg, "Leap_motion_arms_display"sv, "LeapMotion realistic arms"sv, "LeapMotionArmsDisplay"sv, ":/icons/Hand"sv},
        {T::Leap_motion_tracking,     C::Tracking,    TO::V,     CO::N,   true,    R::OpenSource,   S::Leg, "Leap_motion_tracking"sv, "LeapMotion tracking"sv, "LeapMotionTracking"sv, ":/icons/Hand"sv},
        {T::Qualisys,                 C::Tracking,    TO::B,     CO::B,   true,    R::OpenSource,   S::Sta, "Qualisys_tracking"sv, "Qualisys tracking"sv, "QualisysTracking"sv, ":/icons/Qualisys"sv},
        {T::Scene_scaner,             C::Tracking,    TO::B,     CO::B,   true,    R::LNCO,         S::Sta, "Scene_scaner"sv, "Scene scaner"sv, "SceneScaner"sv,":/icons/Body_scanner"sv},
        {T::Sonceboz_SG,              C::Tracking,    TO::U,     CO::B,   true,    R::LNCO,         S::Exp, "Sonceboz_SG"sv, "Sonceboz SG"sv, "SoncebozSG"sv,":/icons/Sonceboz"sv},
        {T::Thera_trainer_tracking,   C::Tracking,    TO::U,     CO::I,   true,    R::LNCO,         S::Sta, "Thera_trainer_tracking"sv, "Thera trainer tracking"sv, "TheraTrainerTracking"sv, ":/icons/Thera_trainer"sv},
        {T::Thera_trainer_platform,   C::Tracking,    TO::B,     CO::C,   false,   R::LNCO,         S::Sta, "Thera_trainer_platform"sv, "Thera trainer platform"sv, "TheraTrainerPlatform"sv, ":/icons/Thera_trainer"sv},
        {T::Vive_pro_eye_tracking,    C::Tracking,    TO::B,     CO::C,   false,   R::OpenSource,   S::Exp, "Vive_pro_eye_tracking"sv, "Vive pro eye tracking"sv, "ViveProEyeTracking"sv, ":/icons/Thera_trainer"sv},
        // UI
        {T::Slider_ui,                C::UI,          TO::B,     CO::B,   false,   R::OpenSource,   S::Sta, "Slider_ui"sv, "Slider ui"sv, "SliderUI"sv, ":/icons/Slider_overlay"sv},
        {T::Buttons_ui,               C::UI,          TO::B,     CO::B,   false,   R::OpenSource,   S::Sta, "Buttons_ui"sv, "Buttons ui"sv, "ButtonsUI"sv, ":/icons/Slider_overlay"sv},
        // Video
        {T::Video_file,               C::Video,       TO::U,     CO::I,   false,   R::OpenSource,   S::Sta, "Video_file"sv, "Video file"sv, "VideoFile"sv,":/icons/Video_file"sv},
        {T::Video_file_camera_viewer, C::Video,       TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Video_file_camera_viewer"sv, "Video file camera viewer"sv, "VideoFileCameraViewer"sv,":/icons/Video_file"sv},
        {T::Video_saver,              C::Video,       TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Video_saver"sv, "Video saver"sv, "VideoSaver"sv,":/icons/Video_record"sv},
        {T::Webcam,                   C::Video,       TO::U,     CO::I,   false,   R::OpenSource,   S::Sta, "Webcam"sv, "Webcam"sv, "Webcam"sv,":/icons/Webcam"sv},
        {T::Volumetric_video,         C::Video,       TO::B,     CO::B,   false,   R::LNCO,         S::Exp, "Volumetric_video"sv,  "Volumetric video"sv, "VolumetricVideo"sv, ":/icons/Video_cloud"sv},
        // Viewer
        {T::Blend_fade_viewer,        C::Viewer,      TO::B,     CO::C,   false,   R::OpenSource,   S::Sta, "Blend_fade_viewer"sv, "Blend fade viewer"sv, "BlendFadeViewer"sv, ":/icons/Blend"sv},
        {T::Fixation_cross_viewer,    C::Viewer,      TO::B,     CO::C,   false,   R::OpenSource,   S::Sta, "Fixation_cross_viewer"sv, "Fixation cross viewer"sv, "FixationCrossViewer"sv, ":/icons/Fixation_cross"sv},
        {T::Fov_simulator_viewer,     C::Viewer,      TO::B,     CO::B,   false,   R::OpenSource,   S::Exp, "Fov_simulator"sv, "FOV simulator"sv, "FovSimulator"sv, ":/icons/FOV"sv},
        {T::Image_viewer,             C::Viewer,      TO::B,     CO::C,   false,   R::OpenSource,   S::Sta, "Image_viewer"sv, "Image viewer"sv, "ImageViewer"sv, ":/icons/Image"sv},
        {T::Text_viewer,              C::Viewer,      TO::B,     CO::C,   false,   R::OpenSource,   S::Sta, "Text_viewer"sv, "Text viewer"sv, "TextViewer"sv, ":/icons/Text_camera"sv},
        {T::Webcam_viewer,            C::Viewer,      TO::B,     CO::B,   false,   R::OpenSource,   S::Sta, "Webcam_viewer"sv, "Webcam viewer"sv, "WebcamViewer"sv, ":/icons/Webcam"sv},
    }};

    static Category get_category(Type type) {
        return components.at<0,1>(type);
    }

    static TO get_timeline_opt(Type type) {
        return components.at<0,2>(type);
    }

    static CO get_config_opt(Type type) {
        return components.at<0,3>(type);
    }

    static bool get_unicity(Type type) {
        return components.at<0,4>(type);
    }

    static Restricted get_restricted(Type type) {
        return components.at<0,5>(type);
    }

    static State get_state(Type type) {
        return components.at<0,6>(type);
    }

    static TypeStr get_type_name(Type type) {
        return components.at<0,7>(type);
    }

    static FullStr get_full_name(Type type) {
        return components.at<0,8>(type);
    }

    static UnityStr get_unity_name(Type type) {
        return components.at<0,9>(type);
    }

    static SV get_icon_path(Type type) {
        return components.at<0,10>(type);
    }

    static size_t components_nb_per_category(Category category) {
        return components.count_equal<1>(category);
    }

    static std::optional<Type> get_type_from_name(FullStr fullName) {
        return components.optional_at<8,0>(fullName);
    }

    static std::optional<Type> get_type_from_unity_name(UnityStr unityName) {
        return components.optional_at<9,0>(unityName);
    }

    static auto all_components_types(){
        return components.tuple_column<0>();
    }


    using CT        = Component::Type;
    using FunctionN = std::string_view;
    using CNT       = ConnectionNode::Type;
    using Doc       = std::string_view;

    struct Info{
        FunctionN functionName;
        ConnectionNode::Type argType;
        Doc documentation;
    };

    using TComponentSlots = std::tuple<
        T,                             FunctionN,                      CNT,                    Doc>;
    static constexpr TupleArray<95,TComponentSlots> componentsSlots = {{
        TComponentSlots
        // Audio
        {T::AudioSource,               "play"sv,                       CNT::void_t,            "..."sv},
        {T::AudioSource,               "pause"sv,                      CNT::void_t,            "..."sv},
        {T::AudioSource,               "set time"sv,                   CNT::float_t,           "..."sv},
        {T::AudioSource,               "set volume"sv,                 CNT::float_t,           "..."sv},
        // Avatar
        {T::Humanoid_avatar,           "init target"sv,                CNT::string_any_t,      "..."sv},
        {T::Humanoid_avatar,           "update target"sv,              CNT::string_any_t,      "..."sv},
        {T::Humanoid_controller,       "init target"sv,                CNT::string_any_t,      "..."sv},
        {T::Humanoid_controller,       "update target"sv,              CNT::string_any_t,      "..."sv},
        // Camera
        {T::Camera,                    "set eye cam"sv,                CNT::transform_t,       "Set the eye camera transform"sv},
        {T::Camera,                    "set neutral cam"sv,            CNT::transform_t,       "Set the start neutral camera transform"sv},
        {T::Camera_target,             "set factor"sv,                 CNT::float_t,           "..."sv},
        // Interaction
        {T::Flag_pole,                 "hoist"sv,                      CNT::float_t,           "Hoist the flag."sv},
        {T::Mark_to_clean,             "erase"sv,                      CNT::gameobject_list_t, "Erase the parts of the mark close to the objects position."sv},
        {T::Mark_to_clean,             "set position"sv,               CNT::void_t,            "Reset the mark"sv},
        // Model
        {T::Cube,                      "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the component"sv},
        {T::Cube,                      "position"sv,                   CNT::vector3_t,         "Set the position of the component"sv},
        {T::Cube,                      "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the component"sv},
        {T::Cube,                      "scale"sv,                      CNT::vector3_t,         "Set the scale of the component"sv},
        {T::Cube,                      "transform"sv,                  CNT::transform_t,       "Set the transform of the component"sv},
        {T::Cylinder,                  "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the component"sv},
        {T::Cylinder,                  "position"sv,                   CNT::vector3_t,         "Set the position of the component"sv},
        {T::Cylinder,                  "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the component"sv},
        {T::Cylinder,                  "scale"sv,                      CNT::vector3_t,         "Set the scale of the component"sv},
        {T::Cylinder,                  "transform"sv,                  CNT::transform_t,       "Set the transform of the component"sv},
        {T::Landmark,                  "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the component"sv},
        {T::Landmark,                  "position"sv,                   CNT::vector3_t,         "Set the position of the component"sv},
        {T::Landmark,                  "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the component"sv},
        {T::Landmark,                  "scale"sv,                      CNT::vector3_t,         "Set the scale of the component"sv},
        {T::Landmark,                  "transform"sv,                  CNT::transform_t,       "Set the transform of the component"sv},
        {T::Torus,                     "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the component"sv},
        {T::Torus,                     "position"sv,                   CNT::vector3_t,         "Set the position of the component"sv},
        {T::Torus,                     "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the component"sv},
        {T::Torus,                     "scale"sv,                      CNT::vector3_t,         "Set the scale of the component"sv},
        {T::Torus,                     "transform"sv,                  CNT::transform_t,       "Set the transform of the component"sv},
        {T::Sphere,                    "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the component"sv},
        {T::Sphere,                    "position"sv,                   CNT::vector3_t,         "Set the position of the component"sv},
        {T::Sphere,                    "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the component"sv},
        {T::Sphere,                    "scale"sv,                      CNT::vector3_t,         "Set the scale of the component"sv},
        {T::Sphere,                    "transform"sv,                  CNT::transform_t,       "Set the transform of the component"sv},
        // Network        
        {T::Parallel_port_writer,      "write"sv,                      CNT::integer_t,         "..."sv},
        {T::Parallel_port_writer,      "send pulse"sv,                 CNT::integer_t,         "..."sv},
        {T::Serial_port_writer,        "send byte pulse"sv,            CNT::integer_t,         "..."sv},
        {T::Serial_port_writer,        "write byte"sv,                 CNT::integer_t,         "..."sv},
        {T::Serial_port_writer,        "write message"sv,              CNT::string_t,          "..."sv},
        {T::Serial_port_writer,        "write line message"sv,         CNT::string_t,          "..."sv},
        {T::Udp_writer,                "send message"sv,               CNT::string_t,          "..."sv},
        // Output
        {T::Logger,                    "write"sv,                      CNT::any_t,             ""sv},
        {T::Logger,                    "write line"sv,                 CNT::any_t,             ""sv},
        {T::Logger,                    "write lines"sv,                CNT::string_list_t,     ""sv},
        {T::LoggerColumns,             "set column value"sv,           CNT::id_any_t,          ""sv},
        {T::LoggerColumns,             "write current line"sv,         CNT::void_t,            ""sv},
        {T::LoggerCondition,           "write"sv,                      CNT::any_t,             ""sv},
        {T::LoggerCondition,           "write line"sv,                 CNT::any_t,             ""sv},
        {T::LoggerCondition,           "write lines"sv,                CNT::string_list_t,     ""sv},
        // Resource
        {T::Text_resource,             "read line"sv,                  CNT::void_t,            "Read next line"sv},
        {T::Image_resource,            "image from alias"sv,           CNT::string_t,          ""sv},
        {T::Image_resource,            "image from id"sv,              CNT::integer_t,         ""sv},
        // Scene
        {T::Unity_asset_bundle,        "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the component"sv},
        {T::Unity_asset_bundle,        "position"sv,                   CNT::vector3_t,         "Set the position of the component"sv},
        {T::Unity_asset_bundle,        "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the component"sv},
        {T::Unity_asset_bundle,        "scale"sv,                      CNT::vector3_t,         "Set the scale of the component"sv},
        {T::Unity_asset_bundle,        "transform"sv,                  CNT::transform_t,       "Set the transform of the component"sv},
        {T::MRI,                       "init knee"sv,                  CNT::string_any_t,      "..."sv},
        {T::MRI,                       "update knee"sv,                CNT::string_any_t,      "..."sv},
        // Script
        {T::CSharp_function,           "input"sv,                      CNT::any_t,             "..."sv},
        {T::CSharp_script,             "slot1"sv,                      CNT::any_t,             "..."sv},
        {T::CSharp_script,             "slot2"sv,                      CNT::any_t,             "..."sv},
        {T::CSharp_script,             "slot3"sv,                      CNT::any_t,             "..."sv},
        {T::CSharp_script,             "slot4"sv,                      CNT::any_t,             "..."sv},
        {T::CSharp_script,             "slot5"sv,                      CNT::id_any_t,          "..."sv},
        {T::Python_script,             "slot1"sv,                      CNT::any_t,             "..."sv},
        {T::Python_script,             "slot2"sv,                      CNT::any_t,             "..."sv},
        {T::Python_script,             "slot3"sv,                      CNT::any_t,             "..."sv},
        {T::Python_script,             "slot4"sv,                      CNT::any_t,             "..."sv},
        // Tracking
        {T::Leap_motion_arms_display,  "trigger"sv,                    CNT::void_t,            "..."sv},
        {T::Leap_motion_arms_display,  "update frame"sv,              CNT::lm_frame_t,        "..."sv},
        {T::Leap_motion_tracking,      "update tracking"sv,            CNT::lm_hands_frame_t,  "..."sv},
        {T::Thera_trainer_platform,    "update rotation"sv,            CNT::vector2_t,         "Set thera trainer platform current rotation angles"sv},
        {T::Biopac,                    "trigger channels"sv,           CNT::void_t,            "..."sv},
        {T::Fop_robot,                 "set delay"sv,                  CNT::integer_t,         "..."sv},
        {T::Fop_robot,                 "set force ratio"sv,            CNT::float_t,           "..."sv},
        // UI
        {T::Buttons_ui,                "set id"sv,                     CNT::integer_t,         "Set the current button with id"sv},
        {T::Buttons_ui,                "next id"sv,                    CNT::void_t,            "Select the new button"sv},
        {T::Buttons_ui,                "previous id"sv,                CNT::void_t,            "Select the previous button"sv},
        {T::Buttons_ui,                "validate"sv,                   CNT::void_t,            "Validate current button"sv},
        {T::Slider_ui,                 "set value"sv,                  CNT::float_t,           "Set the current value of the slider"sv},
        {T::Slider_ui,                 "increase"sv,                   CNT::float_t,           "Increase the slider current value by input amount"sv},
        {T::Slider_ui,                 "decrease"sv,                   CNT::float_t,           "Decrease the slider current value by input amount"sv},
        // Video
        {T::Video_file,                "play"sv,                       CNT::void_t,            "..."sv},
        {T::Video_file,                "pause"sv,                      CNT::void_t,            "..."sv},
        {T::Webcam,                    "play"sv,                       CNT::void_t,            "..."sv},
        {T::Webcam,                    "pause"sv,                      CNT::void_t,            "..."sv},
        {T::Video_saver,               "add frame"sv,                  CNT::image_t,           "..."sv},
        // Viewer
        {T::Image_viewer,              "set image"sv,                  CNT::image_t,           "..."sv},
        {T::Text_viewer,               "set text"sv,                   CNT::string_t,          "..."sv},
    }};

    using TComponentSignals = std::tuple<
        CT,                             FunctionN,                     CNT,                            Doc>;
    static constexpr TupleArray<65, TComponentSignals> componentsSignals = {{
        TComponentSignals
        // Audio
        {T::AudioSource,               "sample value channel"sv,       CNT::id_any_t,                  "..."sv},
        // Avatar
        {T::Humanoid_avatar,           "target transform"sv,           CNT::string_any_t,              "..."sv},
        {T::Humanoid_controller,       "target transform"sv,           CNT::string_any_t,              "..."sv},
        // Camera
        {T::Camera,                    "eye cam"sv,                    CNT::transform_t,               "Eye camera world transform"sv},
        {T::Camera,                    "neutral cam"sv,                CNT::transform_t,               "Start neutral camera world transform"sv},
        // Flow
        {T::Config,                    "current config"sv,             CNT::string_t,                  "Current config of the component"sv},
        // Input
        {T::Mouse,                     "button"sv,                     CNT::mouse_button_event_t,      "Mouse button event"sv},
        {T::Keyboard,                  "button"sv,                     CNT::keyboard_button_event_t,   "Keyboard button event"sv},
        {T::Joypad,                    "button"sv,                     CNT::joypad_button_event_t,     "Joypad button event"sv},
        {T::Joypad,                    "axis"sv,                       CNT::joypad_axis_event_t,       "Joypad axis event"sv},
        // Model
        {T::Cube,                      "visibility changed"sv,         CNT::boolean_t,                 "Is triggered if the visibility of the component changed"sv},
        {T::Cylinder,                  "visibility changed"sv,         CNT::boolean_t,                 "Is triggered if the visibility of the component changed"sv},
        {T::Landmark,                  "visibility changed"sv,         CNT::boolean_t,                 "Is triggered if the visibility of the component changed"sv},
        {T::Torus,                     "visibility changed"sv,         CNT::boolean_t,                 "Is triggered if the visibility of the component changed"sv},
        {T::Sphere,                    "visibility changed"sv,         CNT::boolean_t,                 "Is triggered if the visibility of the component changed"sv},        
        // Network
        {T::Serial_port_reader,        "integer message"sv,            CNT::integer_t,                 "..."sv},
        {T::Serial_port_reader,        "string message"sv,             CNT::string_t,                  "..."sv},
        {T::Udp_reader,                "last message received"sv,      CNT::string_t,                  "..."sv},
        {T::Udp_writer,                "nb bytes sent"sv,              CNT::integer_t,                 "..."sv},                                                                                
        // Resource
        {T::Plot_resource,             "plot loaded"sv,                CNT::plot_t,                    "Loaded plot (called at routine start)"sv},
        {T::Plot_resource,             "plot loaded alias"sv,          CNT::string_t,                  "Alias of the loaded plot (called at routine start)"sv},
        {T::Plot_resource,             "plot loaded path"sv,           CNT::string_t,                  "Path of the loaded plot (called at routine start)"sv},
        {T::Image_resource,            "image"sv,                      CNT::image_t,                   "Loaded image (called at routine start)"sv},
        {T::Image_resource,            "alias"sv,                      CNT::string_t,                  "Alias of the loaded image (called at routine start)"sv},
        {T::Image_resource,            "path"sv,                       CNT::string_t,                  "Path of the loaded image (called at routine start)"sv},
        {T::Text_resource,             "full text"sv,                  CNT::string_t,                  "Loaded text (called at routine start)"sv},
        {T::Text_resource,             "alias"sv,                      CNT::string_t,                  "Alias of the loaded text (called at routine start)"sv},
        {T::Text_resource,             "path"sv,                       CNT::string_t,                  "Path of the loaded text (called at routine start)"sv},
        {T::Text_resource,             "line"sv,                       CNT::string_t,                  "Current read line"sv},
        // Scene
        {T::Unity_asset_bundle,        "visibility changed"sv,         CNT::boolean_t,                 "Is triggered if the visibility of the component changed"sv},
        // Script
        {T::CSharp_function,           "output"sv,                     CNT::any_t,                     "..."sv},
        {T::CSharp_script,             "signal1"sv,                    CNT::any_t,                     "..."sv},
        {T::CSharp_script,             "signal2"sv,                    CNT::any_t,                     "..."sv},
        {T::CSharp_script,             "signal3"sv,                    CNT::any_t,                     "..."sv},
        {T::CSharp_script,             "signal4"sv,                    CNT::any_t,                     "..."sv},
        {T::Python_script,             "signal1"sv,                    CNT::any_t,                     "..."sv},
        {T::Python_script,             "signal2"sv,                    CNT::any_t,                     "..."sv},
        {T::Python_script,             "signal3"sv,                    CNT::any_t,                     "..."sv},
        {T::Python_script,             "signal4"sv,                    CNT::any_t,                     "..."sv},
        // Tracking
        {T::Kinect_body_tracking,      "body"sv,                       CNT::kinect_body_t,             "..."sv},
        {T::Leap_motion,               "new raw frame"sv,              CNT::lm_frame_t,                "..."sv},
        {T::Leap_motion,               "new hands frame"sv,            CNT::lm_hands_frame_t,          "..."sv},
        {T::Leap_motion_arms_display,  "current colliders"sv,          CNT::gameobject_list_t,         "Is triggered when trigger slot is called, will send the current enabled colliders"sv},
        {T::Thera_trainer_tracking,    "new pos"sv,                    CNT::vector2_t,                 "..."sv},
        {T::Thera_trainer_tracking,    "battery"sv,                    CNT::integer_t,                 "..."sv},
        {T::Biopac,                    "channelX last value"sv,        CNT::id_any_t,                  "Send real associated to id channel"sv},
        {T::Biopac,                    "channelX last range values"sv, CNT::id_any_t,                  "Send reals list associated to id channel"sv},
        {T::Biopac,                    "end routine data log"sv,       CNT::string_list_t,             "..."sv},
        {T::Biopac,                    "channels latency"sv,           CNT::real_t,                    "..."sv},
        {T::Qualisys,                  "tracked object"sv,             CNT::string_any_t,              "..."sv},
        {T::Fop_robot,                 "slave position"sv,             CNT::vector3_t,                 "..."sv},
        {T::Fop_robot,                 "master position"sv,            CNT::vector3_t,                 "..."sv},
        {T::Fop_robot,                 "slave touch"sv,                CNT::boolean_t,                 "..."sv},
        {T::Fop_robot,                 "master touch"sv,               CNT::boolean_t,                 "..."sv},
        {T::Fop_robot,                 "slave force"sv,                CNT::float_t,                   "..."sv},
        {T::Fop_robot,                 "master force"sv,               CNT::float_t,                   "..."sv},
        {T::Sonceboz_SG,               "feedback"sv,                   CNT::transform_t,               "..."sv},
        {T::Vive_pro_eye_tracking,     "gaze direction"sv,             CNT::vector3_t,                 "..."sv},
        {T::Vive_pro_eye_tracking,     "eye openess"sv,                CNT::id_any_t,                  "..."sv},
        {T::Vive_pro_eye_tracking,     "pupil position"sv,             CNT::id_any_t,                  "..."sv},
        {T::Vive_pro_eye_tracking,     "pupil diameter"sv,             CNT::id_any_t,                  "..."sv},
        // UI
        {T::Buttons_ui,                "validated"sv,                  CNT::integer_t,                 "Current selected button has been validated"sv},
        {T::Slider_ui,                 "value updated"sv,              CNT::float_t,                   "Is triggered when slider value changes"sv},
        // Video
        {T::Video_file,                "new frame"sv,                  CNT::image_t,                   "..."sv},
        {T::Webcam,                    "new frame"sv,                  CNT::image_t,                   "..."sv},
    }};


    static bool has_slots(CT type) {
        return componentsSlots.count_equal<0>(type) != 0;
    }

    static size_t slots_count(CT type) {
        return componentsSlots.count_equal<0>(type);
    }

    static auto get_slots(CT type) {
        return componentsSlots.tuples_matching_columns_values<0>(type);
    }

    static std::vector<FunctionN> get_slots_functions(CT type) {
        return componentsSlots.elements_matching_columns_values<0,1>(type);
    }

    static bool has_signals(CT type) {
        return componentsSignals.count_equal<0>(type) != 0;
    }

    static size_t signals_count(CT type) {
        return componentsSignals.count_equal<0>(type);
    }

    static auto get_signals(CT type) {
        return componentsSignals.tuples_matching_columns_values<0>(type);
    }

    static std::vector<FunctionN> get_signals_functions(CT type) {
        return componentsSignals.elements_matching_columns_values<0,1>(type);
    }

    Component() = delete;
    Component(Type t, ComponentKey id, QString name);
    ~Component();

    inline void set_init_config(ConfigUP config){
        initConfig = std::move(config);
    }

    inline void add_config(ConfigUP config){
        configs.emplace_back(std::move(config));
    }

    static ComponentUP copy_with_new_element_id(const Component &componentToCopy, const QString &newName);

    QStringList get_configs_name() const;
    inline QString name() const{return m_name;}
    void set_name(QString name);

    tool::ex::Config *get_config(ConfigKey configKey);

    IdKey key;


    inline QString to_string() const{return QSL("Component(") % m_name % QSL("|") %
               from_view(get_full_name(type)) % QSL("|") % QString::number(key()) % QSL(")");}

    static inline size_t count(Type t) {return m_counter[t];}

private:

    QString m_name = "default";
    static inline std::map<Type, size_t> m_counter = {};

public:

    Type type = Type::SizeEnum;
    Category category   = Category::SizeEnum;
    ConfigUP initConfig = nullptr;

    RowId selectedConfigId = {0};
    std_v1<ConfigUP> configs;    
};
}
