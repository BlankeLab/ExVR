
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
#include "config.hpp"
#include "connection_node.hpp"


namespace tool::ex {

using namespace std::literals::string_view_literals;

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

    [[maybe_unused]] static constexpr Name to_string(Category c) {
        return categories.at<0,1>(c);
    }
    [[maybe_unused]] static constexpr std::optional<Category> get_category(const Name name) {
        return categories.optional_at<1,0>(name);
    }

    static constexpr auto all_categories() {
        return categories.tuple_column<0>();
    }

    static constexpr auto all_categories_name(){
        return categories.tuple_column<1>();
    }

    enum class Type : int {
        /** A */ AudioListener, AudioSource, Attach_object_to_hand,
        /** B */ Biopac, Blend_fade_viewer, Buttons_ui,
        /** C */ Camera, Camera_target, Camera_trajectory, Camera_trajectory_file, Cloud, Config, CSharp_function, CSharp_script, Cube, Cylinder,
        /** F */ Falling_spheres, Fixation_cross_viewer, Flag_pole, Flashing_dot, Fop_robot, Fov_simulator_viewer, FPP_avatar_camera, FPP_camera,
        /** G */ Global_logger,
        /** H */ Humanoid_avatar, Humanoid_controller,
        /** I */ Image_resource, Image_viewer,
        /** J */ Joypad,
        /** K */ Keyboard, K4_manager, K2_manager, K2_body_tracking,
        /** L */ Landmark, Leap_motion, Leap_motion_arms_display, Leap_motion_tracking, Lines, Logger, LoggerColumns, LoggerCondition, LoggerExperiment,
        /** M */ Mark_to_clean, Microphone, Mirror, Mouse, MRI, Multi_AB,
        /** P */ Parallel_port_writer, Plane, Plot_resource, Post_process, Python_script,
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
        OpenSource, ClosedSource, LNCO,
        OS = OpenSource, CS = ClosedSource, LN = LNCO
    };
    using R = Restricted;

    enum class State : int {
        Sta, Exp, Leg
    };    
    using S = State;

    enum class Priority : int{
        Low, Medium, Hight,
        L = Low, M = Medium, H = Hight
    };
    using P = Priority;

    static constexpr bool Y = true;
    static constexpr bool N = false;

    using SV                   = std::string_view;
    using TypeStr              = SV;
    using FullStr              = SV;
    using UnityStr             = SV;
    using IconStr              = SV;
    using Unicity              = bool;
    using Global               = bool;
    using AlwaysUpdate         = bool;
    using Exceptions           = bool;
    using CanHasFrameLogging   = bool;
    using CanHasTriggerLogging = bool;

    static constexpr size_t ColType              = 0;
    static constexpr size_t ColCategory          = 1;
    static constexpr size_t ColTimelineO         = 2;
    static constexpr size_t ColConfigO           = 3;
    static constexpr size_t ColUnicity           = 4;
    static constexpr size_t ColGlobal            = 5;
    static constexpr size_t ColAlwaysUpdate      = 6;
    static constexpr size_t ColExceptions        = 7;
    static constexpr size_t ColHasFrameLogging   = 8;
    static constexpr size_t ColHasTriggerLogging = 9;
    static constexpr size_t ColPriority          = 10;
    static constexpr size_t ColRestricted        = 11;
    static constexpr size_t ColState             = 12;
    static constexpr size_t ColTypeStr           = 13;
    static constexpr size_t ColFullStr           = 14;
    static constexpr size_t ColUnityStr          = 15;
    static constexpr size_t ColIconStr           = 16;

    using TComponent = std::tuple<
        Type, Category, TimelineO, ConfigO, Unicity, Global, AlwaysUpdate, Exceptions, CanHasFrameLogging, CanHasTriggerLogging,
    //  0     1         2          3        4        5       6             7           8                   9
        Priority, Restricted, State, TypeStr, FullStr, UnityStr, IconStr>;
    //  10        11          12     13       14       15        16
    static constexpr TupleArray<T::SizeEnum, TComponent> components ={{
        TComponent
        // 0                          1               2      3      4  5  6  7  8  9  10    11     12      13 14 15 16
        // Audio
        {T::AudioListener,            C::Audio,       TO::U, CO::B, Y, N, N, Y, N, N, P::M, R::OS, S::Exp, "AudioListener"sv, "Audio listener"sv, "AudioListener"sv, "Sound"sv},
        {T::AudioSource,              C::Audio,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Sta, "AudioSource"sv, "Audio source"sv, "AudioSource"sv, "Sound"sv},
        {T::Microphone,               C::Audio,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Microphone"sv, "Microphone"sv, "Microphone"sv, "Micro"sv},
        // Avatar
        {T::Humanoid_avatar,          C::Avatar,      TO::B, CO::B, N, N, N, Y, N, N, P::M, R::CS, S::Sta, "Humanoid_avatar"sv, "Humanoid avatar"sv, "HumanoidAvatar"sv, "Avatar"sv},
        {T::Humanoid_controller,      C::Avatar,      TO::B, CO::B, N, N, N, Y, N, N, P::L, R::CS, S::Leg, "Humanoid_controller"sv, "Humanoid controller"sv, "HumanoidController"sv, "Avatar"sv},
        // Camera
        {T::Camera,                   C::Camera,      TO::U, CO::B, Y, N, N, Y, N, N, P::L, R::OS, S::Sta, "Camera"sv, "Camera"sv, "Camera"sv, "Camera"sv},
        {T::Camera_target,            C::Camera,      TO::U, CO::C, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "Camera_target"sv, "Camera target"sv, "CameraTarget"sv, "Camera"sv},
        {T::Camera_trajectory,        C::Camera,      TO::B, CO::C, N, N, N, Y, N, N, P::L, R::OS, S::Leg, "Camera_trajectory"sv, "Camera trajectory"sv, "CameraTrajectory"sv, "Camera"sv},
        {T::Camera_trajectory_file,   C::Camera,      TO::B, CO::C, N, N, N, Y, N, N, P::L, R::OS, S::Leg, "Camera_trajectory_file"sv, "Camera trajectory file"sv, "CameraTrajectoryFile"sv, "Camera"sv},
        {T::FPP_avatar_camera,        C::Camera,      TO::U, CO::B, N, N, N, Y, N, N, P::L, R::CS, S::Sta, "FPP_avatar_camera"sv, "First person perspective avatar camera"sv, "FPPAvatarCamera"sv, "Camera"sv},
        {T::TPP_avatar_camera,        C::Camera,      TO::U, CO::B, N, N, N, Y, N, N, P::L, R::CS, S::Sta, "TPP_avatar_camera"sv, "Third persond perspective avatar camera"sv, "TPPAvatarCamera"sv, "Camera"sv},
        {T::FPP_camera,               C::Camera,      TO::U, CO::B, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "FPP_camera"sv, "First person perspective camera"sv, "FPPCamera"sv, "Camera"sv},
        // Cloud
        {T::Cloud,                    C::Cloud,       TO::V, CO::B, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Cloud"sv, "Cloud"sv, "Cloud"sv, "Cloud"sv},
        {T::Scaner_video,             C::Cloud,       TO::B, CO::B, N, N, N, Y, N, N, P::H, R::LN, S::Exp, "Scaner_video"sv, "Scaner video"sv, "ScanerVideo"sv, "Video_cloud"sv},
        // Environment
        {T::Post_process,             C::Environment, TO::N, CO::C, Y, N, N, Y, N, N, P::H, R::OS, S::Exp, "Post_porcess"sv, "Post process"sv, "PostProcess"sv, "Post_process"sv},
        {T::Sky,                      C::Environment, TO::N, CO::C, Y, N, N, Y, N, N, P::H, R::OS, S::Sta, "Sky"sv, "Sky"sv, "Sky"sv, "Sky"sv},
        // Flow
        {T::Config,                   C::Flow,        TO::N, CO::C, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Config"sv, "Config"sv, "Config"sv, "Config"sv},
        // Input
        {T::Joypad,                   C::Input,       TO::U, CO::I, Y, Y, Y, N, Y, Y, P::H, R::OS, S::Sta, "Joypad"sv, "Joypad"sv, "Joypad"sv,"Joypad"sv},
        {T::Keyboard,                 C::Input,       TO::U, CO::I, Y, Y, Y, N, Y, Y, P::H, R::OS, S::Sta, "Keyboard"sv, "Keyboard"sv, "Keyboard"sv, "Keyboard"sv},
        {T::Mouse,                    C::Input,       TO::U, CO::I, Y, Y, Y, N, Y, Y, P::H, R::OS, S::Sta, "Mouse"sv, "Mouse"sv, "Mouse"sv, "Mouse"sv},
        // Interaction
        {T::Flag_pole,                C::Interaction, TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Sta, "Flag_pole"sv, "Flag pole"sv, "FlagPole"sv, "Flag_pole"sv},
        {T::Mark_to_clean,            C::Interaction, TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Mark_to_clean"sv, "Mark to clean"sv, "MarkToClean"sv, "Mark_to_clean"sv},
        {T::Target_to_grab,           C::Interaction, TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Target_to_grab"sv, "Target to grab"sv, "TargetToGrab"sv, "Grab_target"sv},
        // Model
 /**OK*/{T::Cube,                     C::Model,       TO::V, CO::B, N, N, N, N, N, N, P::M, R::OS, S::Sta, "Cube"sv, "Cube"sv, "Cube"sv,"Cube"sv},
 /**OK*/{T::Cylinder,                 C::Model,       TO::V, CO::B, N, N, N, N, N, N, P::M, R::OS, S::Sta, "Cylinder"sv, "Cylinder"sv, "Cylinder"sv,"Cylinder"sv},
 /**OK*/{T::Landmark,                 C::Model,       TO::V, CO::B, N, N, N, N, N, N, P::M, R::OS, S::Sta, "Landmark"sv, "Landmark"sv, "Landmark"sv,"Landmark"sv},
        {T::Lines,                    C::Model,       TO::V, CO::C, N, N, N, N, N, N, P::M, R::OS, S::Exp, "Lines"sv, "Lines"sv, "Lines"sv,"Line"sv},
 /**OK*/{T::Plane,                    C::Model,       TO::V, CO::B, N, N, N, N, N, N, P::M, R::OS, S::Sta, "Plane"sv, "Plane"sv, "Plane"sv,"Plane"sv},
 /**OK*/{T::Sphere,                   C::Model,       TO::V, CO::B, N, N, N, N, N, N, P::M, R::OS, S::Sta, "Sphere"sv, "Sphere"sv, "Sphere"sv,"Sphere"sv},
 /**OK*/{T::Torus,                    C::Model,       TO::V, CO::B, N, N, N, N, N, N, P::M, R::OS, S::Sta, "Torus"sv, "Torus"sv, "Torus"sv,"Torus"sv},
        // Network
        {T::Parallel_port_writer,     C::Network,     TO::N, CO::B, N, N, Y, N, N, Y, P::H, R::OS, S::Sta, "Parallel_port_writer"sv, "Parallel port writer"sv, "ParallelPortWriter"sv, "Parallel_port"sv},
        {T::Serial_port_reader,       C::Network,     TO::U, CO::B, N, N, Y, N, N, Y, P::H, R::OS, S::Sta, "Serial_port_reader"sv, "Serial port reader"sv, "SerialPortReader"sv, "USB"sv},
        {T::Serial_port_writer,       C::Network,     TO::U, CO::B, N, N, Y, N, N, Y, P::H, R::OS, S::Sta, "Serial_port_writer"sv, "Serial port writer"sv, "SerialPortWriter"sv, "USB"sv},
        {T::Udp_reader,               C::Network,     TO::U, CO::I, N, N, Y, N, N, Y, P::H, R::OS, S::Sta, "Udp_reader"sv, "UDP reader"sv, "UdpReader"sv, "UDP"sv},
        {T::Udp_writer,               C::Network,     TO::U, CO::I, N, N, Y, N, N, Y, P::H, R::OS, S::Sta, "Udp_writer"sv, "UDP writer"sv, "UdpWriter"sv, "UDP"sv},
        // Output
        {T::Global_logger,            C::Output,      TO::N, CO::I, Y, Y, Y, N, N, N, P::H, R::OS, S::Sta, "Global_logger"sv, "Global logger"sv, "GlobalLogger"sv, "Logger"sv},
        {T::Logger,                   C::Output,      TO::N, CO::I, N, N, N, N, N, N, P::H, R::OS, S::Sta, "Logger"sv, "Logger"sv, "Logger"sv, "Logger"sv},
        {T::LoggerColumns,            C::Output,      TO::N, CO::I, N, N, N, N, N, N, P::H, R::OS, S::Sta, "Logger_columns"sv, "Logger columns"sv, "LoggerColumns"sv, "Logger"sv},
        {T::LoggerCondition,          C::Output,      TO::N, CO::I, N, N, N, N, N, N, P::H, R::OS, S::Sta, "Logger_condition"sv, "Logger condition"sv, "LoggerCondition"sv, "Logger"sv},
        {T::LoggerExperiment,         C::Output,      TO::N, CO::I, N, N, N, N, N, N, P::H, R::OS, S::Leg, "Logger_experiment"sv, "Logger experiment"sv, "LoggerExperiment"sv, "Logger"sv},
        // Resource
        {T::Image_resource,           C::Resource,    TO::N, CO::B, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Image_resource"sv, "Image resource"sv, "ImageResource"sv, "Image"sv},
        {T::Plot_resource,            C::Resource,    TO::N, CO::C, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Plot_resource"sv,  "Plot resource"sv, "PlotResource"sv, "Plot"sv},
        {T::Text_resource,            C::Resource,    TO::N, CO::C, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Text_resource"sv,  "Text resource"sv, "TextResource"sv, "Text"sv},
        // Scene
        {T::Falling_spheres,          C::Scene,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Falling_spheres"sv, "Falling spheres"sv, "FallingSpheres"sv, "Falling_spheres"sv},
        {T::Flashing_dot,             C::Scene,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Flashing_dot"sv, "Flashing dot"sv, "FlashingDot"sv,"Dot"sv},
        {T::Mirror,                   C::Scene,       TO::V, CO::C, N, N, N, Y, N, N, P::M, R::OS, S::Leg, "Mirror"sv, "Mirror"sv, "Mirror"sv, "Mirror"sv},
        {T::MRI,                      C::Scene,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::CS, S::Exp, "MRI"sv, "MRI"sv, "MRI"sv, "MRI"sv},
        {T::Multi_AB,                 C::Scene,       TO::V, CO::B, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Multi_AB"sv, "Multi assets bundles"sv, "MultiAB"sv, "Unity_scene_bundle"sv},
        {T::Unity_asset_bundle,       C::Scene,       TO::V, CO::B, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Unity_asset_bundle"sv, "Unity asset bundle"sv, "AssetBundle"sv, "Unity_scene_bundle"sv},
        // Script
        {T::CSharp_function,          C::Script,      TO::N, CO::B, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "CSharp_function"sv, "CSharp function"sv, "CSharpFunction"sv, "CSharp"sv},
        {T::CSharp_script,            C::Script,      TO::B, CO::B, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "CSharp_script"sv, "CSharp script"sv, "CSharpScript"sv, "CSharp"sv},
        {T::Python_script,            C::Script,      TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Python_script"sv, "Python script"sv, "PythonScript"sv, "Python_script"sv},
        // Tracking
        {T::Attach_object_to_hand,    C::Tracking,    TO::B, CO::B, Y, N, N, Y, N, N, P::M, R::OS, S::Exp, "Attach_object_to_hand"sv, "Attach object to hand"sv, "AttachObjectToHand"sv, "Sponge"sv},
        {T::Biopac,                   C::Tracking,    TO::U, CO::I, Y, N, N, Y, N, N, P::H, R::OS, S::Sta, "Biopac"sv, "Biopac device"sv, "Biopac"sv, "Physio"sv},
        {T::Fop_robot,                C::Tracking,    TO::U, CO::B, Y, N, N, Y, N, N, P::H, R::LN, S::Sta, "Fop_robot"sv, "FOP robot"sv, "FOPRobot"sv,"Fop_robot"sv},        
        {T::K2_body_tracking,         C::Tracking,    TO::B, CO::B, Y, N, N, Y, N, N, P::H, R::LN, S::Exp, "K2_body_tracking"sv, "K2 body tracking"sv, "K2BodyTracking"sv,"Kinect"sv},
        {T::K2_manager,               C::Tracking,    TO::U, CO::B, Y, N, N, Y, N, N, P::H, R::LN, S::Sta, "K2_manager"sv, "K2 manager"sv, "K2Manager"sv,"Kinect"sv},
        {T::K4_manager,               C::Tracking,    TO::U, CO::B, Y, N, N, Y, N, N, P::H, R::LN, S::Sta, "K4_manager"sv, "K4 manager"sv, "K4Manager"sv,"Kinect"sv},
        {T::Leap_motion,              C::Tracking,    TO::U, CO::C, Y, Y, Y, Y, Y, N, P::H, R::OS, S::Sta, "Leap_motion"sv, "LeapMotion"sv, "LeapMotion"sv, "Hand"sv},
        {T::Leap_motion_arms_display, C::Tracking,    TO::V, CO::B, Y, N, N, Y, N, N, P::M, R::CS, S::Leg, "Leap_motion_arms_display"sv, "LeapMotion realistic arms"sv, "LeapMotionArmsDisplay"sv, "Hand"sv},
        {T::Leap_motion_tracking,     C::Tracking,    TO::V, CO::N, Y, N, N, Y, N, N, P::M, R::OS, S::Leg, "Leap_motion_tracking"sv, "LeapMotion tracking"sv, "LeapMotionTracking"sv, "Hand"sv},
        {T::Qualisys,                 C::Tracking,    TO::B, CO::B, Y, Y, Y, Y, Y, N, P::M, R::OS, S::Sta, "Qualisys_tracking"sv, "Qualisys tracking"sv, "QualisysTracking"sv, "Qualisys"sv},
        {T::Scene_scaner,             C::Tracking,    TO::B, CO::B, Y, N, N, Y, N, N, P::M, R::LN, S::Sta, "Scene_scaner"sv, "Scene scaner"sv, "SceneScaner"sv,"Body_scanner"sv},
        {T::Sonceboz_SG,              C::Tracking,    TO::U, CO::B, Y, Y, Y, Y, Y, N, P::H, R::LN, S::Sta, "Sonceboz_SG"sv, "Sonceboz SG"sv, "SoncebozSG"sv,"Sonceboz"sv},
        {T::Thera_trainer_tracking,   C::Tracking,    TO::U, CO::I, Y, N, N, Y, N, N, P::M, R::LN, S::Sta, "Thera_trainer_tracking"sv, "Thera trainer tracking"sv, "TheraTrainerTracking"sv, "Thera_trainer"sv},
        {T::Thera_trainer_platform,   C::Tracking,    TO::B, CO::C, N, N, N, Y, N, N, P::M, R::LN, S::Sta, "Thera_trainer_platform"sv, "Thera trainer platform"sv, "TheraTrainerPlatform"sv, "Thera_trainer"sv},
        {T::Vive_pro_eye_tracking,    C::Tracking,    TO::B, CO::C, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Vive_pro_eye_tracking"sv, "Vive pro eye tracking"sv, "ViveProEyeTracking"sv, "Thera_trainer"sv},
        // UI
        {T::Slider_ui,                C::UI,          TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Sta, "Slider_ui"sv, "Slider ui"sv, "SliderUI"sv, "Slider_overlay"sv},
        {T::Buttons_ui,               C::UI,          TO::B, CO::C, N, N, N, Y, N, N, P::M, R::OS, S::Sta, "Buttons_ui"sv, "Buttons ui"sv, "ButtonsUI"sv, "Buttons"sv},
        // Video
        {T::Video_file,               C::Video,       TO::U, CO::I, N, N, N, Y, N, N, P::M, R::OS, S::Sta, "Video_file"sv, "Video file"sv, "VideoFile"sv,"Video_file"sv},
        {T::Video_file_camera_viewer, C::Video,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Video_file_camera_viewer"sv, "Video file camera viewer"sv, "VideoFileCameraViewer"sv,"Video_file"sv},
        {T::Video_saver,              C::Video,       TO::B, CO::B, N, N, N, Y, N, N, P::M, R::OS, S::Exp, "Video_saver"sv, "Video saver"sv, "VideoSaver"sv,"Video_record"sv},
        {T::Webcam,                   C::Video,       TO::U, CO::I, N, N, N, Y, N, N, P::M, R::OS, S::Sta, "Webcam"sv, "Webcam"sv, "Webcam"sv,"Webcam"sv},
        {T::Volumetric_video,         C::Video,       TO::B, CO::B, N, N, N, Y, N, N, P::H, R::LN, S::Exp, "Volumetric_video"sv,  "Volumetric video"sv, "VolumetricVideo"sv, "Video_cloud"sv},
        // Viewer
        {T::Blend_fade_viewer,        C::Viewer,      TO::B, CO::C, N, N, N, Y, N, N, P::H, R::OS, S::Sta, "Blend_fade_viewer"sv, "Blend fade viewer"sv, "BlendFadeViewer"sv, "Blend"sv},
        {T::Fixation_cross_viewer,    C::Viewer,      TO::B, CO::C, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "Fixation_cross_viewer"sv, "Fixation cross viewer"sv, "FixationCrossViewer"sv, "Fixation_cross"sv},
        {T::Fov_simulator_viewer,     C::Viewer,      TO::B, CO::B, N, N, N, Y, N, N, P::H, R::OS, S::Exp, "Fov_simulator"sv, "FOV simulator"sv, "FovSimulator"sv, "FOV"sv},
        {T::Image_viewer,             C::Viewer,      TO::B, CO::C, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "Image_viewer"sv, "Image viewer"sv, "ImageViewer"sv, "Image"sv},
        {T::Text_viewer,              C::Viewer,      TO::B, CO::C, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "Text_viewer"sv, "Text viewer"sv, "TextViewer"sv, "Text_camera"sv},
        {T::Webcam_viewer,            C::Viewer,      TO::B, CO::B, N, N, N, Y, N, N, P::L, R::OS, S::Sta, "Webcam_viewer"sv, "Webcam viewer"sv, "WebcamViewer"sv, "Webcam"sv},
    }};

    static constexpr Category get_category(Type type) {
        return components.at<0,ColCategory>(type);
    }

    static constexpr TO get_timeline_opt(Type type) {
        return components.at<0,ColTimelineO>(type);
    }

    static constexpr CO get_config_opt(Type type) {
        return components.at<0,ColConfigO>(type);
    }

    static constexpr bool get_unicity(Type type) {
        return components.at<0,ColUnicity>(type);
    }

    static constexpr bool is_global(Type type) {
        return components.at<0,ColGlobal>(type);
    }

    static constexpr bool is_alsways_updating(Type type) {
        return components.at<0,ColAlwaysUpdate>(type);
    }

    static constexpr bool get_exceptions(Type type) {
        return components.at<0,ColExceptions>(type);
    }

    static constexpr bool has_frame_logging(Type type) {
        return components.at<0,ColHasFrameLogging>(type);
    }

    static constexpr bool has_trigger_logging(Type type) {
        return components.at<0,ColHasTriggerLogging>(type);
    }

    static constexpr Priority get_priority(Type type) {
        return components.at<0,ColPriority>(type);
    }

    static constexpr Restricted get_restricted(Type type) {
        return components.at<0,ColRestricted>(type);
    }

    static constexpr State get_state(Type type) {
        return components.at<0,ColState>(type);
    }

    static constexpr TypeStr get_type_name(Type type) {
        return components.at<0,ColTypeStr>(type);
    }

    static constexpr FullStr get_full_name(Type type) {
        return components.at<0,ColFullStr>(type);
    }

    static constexpr UnityStr get_unity_name(Type type) {
        return components.at<0,ColUnityStr>(type);
    }

    static constexpr SV get_icon_id(Type type) {
        return components.at<0,ColIconStr>(type);
    }

    static QString get_icon_path(Type type) {
        return QSL(":/icons/") % from_view(get_icon_id(type));
    }

    static constexpr size_t components_nb_per_category(Category category) {
        return components.count_equal<ColCategory>(category);
    }

    static constexpr std::optional<Type> get_type_from_name(FullStr fullName) {
        return components.optional_at<ColFullStr,ColType>(fullName);
    }

    static constexpr std::optional<Type> get_type_from_unity_name(UnityStr unityName) {
        return components.optional_at<ColUnityStr,ColType>(unityName);
    }

    static constexpr auto all_components_types(){
        return components.tuple_column<ColType>();
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
    static constexpr TupleArray<109,TComponentSlots> componentsSlots = {{
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
        {T::Camera_target,             "set target offset pos"sv,      CNT::vector3_t,         "..."sv},
        {T::Camera_target,             "set target offset rot"sv,      CNT::vector3_t,         "..."sv},
        // Interaction
        {T::Flag_pole,                 "hoist"sv,                      CNT::float_t,           "Hoist the flag."sv},
        {T::Mark_to_clean,             "erase"sv,                      CNT::gameobject_list_t, "Erase the parts of the mark close to the objects position."sv},
        {T::Mark_to_clean,             "set position"sv,               CNT::void_t,            "Reset the mark"sv},
        // Model
        {T::Cube,                      "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the model"sv},
        {T::Cube,                      "position"sv,                   CNT::vector3_t,         "Set the position of the model"sv},
        {T::Cube,                      "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the model"sv},
        {T::Cube,                      "scale"sv,                      CNT::vector3_t,         "Set the scale of the model"sv},
        {T::Cube,                      "transform"sv,                  CNT::transform_t,       "Set the transform of the model"sv},
        {T::Cube,                      "RGBA texture"sv,               CNT::image_t,           "Set the RGBA texture of the model"sv},
        {T::Plane,                     "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the model"sv},
        {T::Plane,                     "position"sv,                   CNT::vector3_t,         "Set the position of the model"sv},
        {T::Plane,                     "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the model"sv},
        {T::Plane,                     "scale"sv,                      CNT::vector3_t,         "Set the scale of the model"sv},
        {T::Plane,                     "transform"sv,                  CNT::transform_t,       "Set the transform of the model"sv},
        {T::Plane,                     "RGBA texture"sv,               CNT::image_t,           "Set the RGBA texture of the model"sv},
        {T::Cylinder,                  "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the model"sv},
        {T::Cylinder,                  "position"sv,                   CNT::vector3_t,         "Set the position of the model"sv},
        {T::Cylinder,                  "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the model"sv},
        {T::Cylinder,                  "scale"sv,                      CNT::vector3_t,         "Set the scale of the model"sv},
        {T::Cylinder,                  "transform"sv,                  CNT::transform_t,       "Set the transform of the model"sv},
        {T::Cylinder,                  "RGBA texture"sv,               CNT::image_t,           "Set the RGBA texture of the model"sv},
        {T::Landmark,                  "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the model"sv},
        {T::Landmark,                  "position"sv,                   CNT::vector3_t,         "Set the position of the model"sv},
        {T::Landmark,                  "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the model"sv},
        {T::Landmark,                  "scale"sv,                      CNT::vector3_t,         "Set the scale of the model"sv},
        {T::Landmark,                  "transform"sv,                  CNT::transform_t,       "Set the transform of the model"sv},
        {T::Torus,                     "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the model"sv},
        {T::Torus,                     "position"sv,                   CNT::vector3_t,         "Set the position of the model"sv},
        {T::Torus,                     "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the model"sv},
        {T::Torus,                     "scale"sv,                      CNT::vector3_t,         "Set the scale of the model"sv},
        {T::Torus,                     "transform"sv,                  CNT::transform_t,       "Set the transform of the model"sv},
        {T::Torus,                     "RGBA texture"sv,               CNT::image_t,           "Set the RGBA texture of the model"sv},
        {T::Sphere,                    "visibility"sv,                 CNT::boolean_t,         "Set the visibility of the model"sv},
        {T::Sphere,                    "position"sv,                   CNT::vector3_t,         "Set the position of the model"sv},
        {T::Sphere,                    "rotation"sv,                   CNT::vector3_t,         "Set the rotation (euler angles) of the model"sv},
        {T::Sphere,                    "scale"sv,                      CNT::vector3_t,         "Set the scale of the model"sv},
        {T::Sphere,                    "transform"sv,                  CNT::transform_t,       "Set the transform of the model"sv},
        {T::Sphere,                    "RGBA texture"sv,               CNT::image_t,           "Set the RGBA texture of the model"sv},
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
        {T::LoggerColumns,             "reset all values"sv,           CNT::void_t,            ""sv},
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
        {T::Leap_motion_arms_display,  "update frame"sv,               CNT::lm_frame_t,        "..."sv},
        {T::Leap_motion_tracking,      "update tracking"sv,            CNT::lm_hands_frame_t,  "..."sv},
        {T::Thera_trainer_platform,    "update rotation"sv,            CNT::vector2_t,         "Set thera trainer platform current rotation angles"sv},
        {T::Biopac,                    "inused"sv,                     CNT::void_t,            "TODO: Remove it"sv},
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
        {T::Text_viewer,               "set color"sv,                  CNT::color_t,           "..."sv},
    }};

    using TComponentSignals = std::tuple<
        CT,                             FunctionN,                     CNT,                            Doc>;
    static constexpr TupleArray<63, TComponentSignals> componentsSignals = {{
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
        {T::Mouse,                     "axis"sv,                       CNT::mouse_axis_event_t,        "Mouse axis event"sv},
        {T::Mouse,                     "position"sv,                   CNT::vector2_t,                 "Mouse position"sv},
        {T::Keyboard,                  "button"sv,                     CNT::keyboard_button_event_t,   "Keyboard button event"sv},
        {T::Joypad,                    "button"sv,                     CNT::joypad_button_event_t,     "Joypad button event"sv},
        {T::Joypad,                    "axis"sv,                       CNT::joypad_axis_event_t,       "Joypad axis event"sv},
        // Model
        // Network
        {T::Parallel_port_writer,      "message sent"sv,               CNT::time_any_t,                "..."sv},
        {T::Serial_port_writer,        "message sent"sv,               CNT::time_any_t,                "..."sv},
        {T::Serial_port_reader,        "message read"sv,               CNT::time_any_t,                "..."sv},
        {T::Udp_reader,                "message read"sv,               CNT::time_any_t,                "..."sv},
        {T::Udp_writer,                "message sent"sv,               CNT::time_any_t,                "..."sv},
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
        {T::K2_body_tracking,      "body"sv,                       CNT::kinect_body_t,             "..."sv},
        {T::Leap_motion,               "new raw frame"sv,              CNT::lm_frame_t,                "..."sv},
        {T::Leap_motion,               "new hands frame"sv,            CNT::lm_hands_frame_t,          "..."sv},
        {T::Leap_motion_arms_display,  "current colliders"sv,          CNT::gameobject_list_t,         "Is triggered when trigger slot is called, will send the current enabled colliders"sv},
        {T::Thera_trainer_tracking,    "new pos"sv,                    CNT::vector2_t,                 "..."sv},
        {T::Thera_trainer_tracking,    "battery"sv,                    CNT::integer_t,                 "..."sv},
        {T::Biopac,                    "channelX last value"sv,        CNT::id_any_t,                  "Send last data value corresponding to id channel"sv},
        {T::Biopac,                    "channelX last range values"sv, CNT::id_any_t,                  "Send last range data value corresponding to id channel"sv},
        {T::Biopac,                    "call duration API"sv,          CNT::real_t,                   "..."sv},
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
        {T::Buttons_ui,                "validated id"sv,               CNT::integer_t,                 "Button with id has been validated"sv},
        {T::Buttons_ui,                "validated text"sv,             CNT::string_t,                  "Button with text has been validated"sv},
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
    Component(Type t, ComponentKey id, QString name, std::unique_ptr<Config> initConfig);
    Component(const Component &) = delete;
    Component& operator=(const Component&) = delete;

    static std::unique_ptr<Component> copy_with_new_element_id(Component *componentToCopy, const QString &newName, std::vector<ConfigKey> configKeys = {});

    inline QString name() const noexcept{return m_name;}
    inline void set_name(QString name) noexcept{m_name = name;}

    void add_config(std::unique_ptr<Config> config);
    bool insert_config(RowId id, QString configName);
    bool select_config(RowId id);
    bool remove_config(RowId id);
    bool move_config(RowId from, RowId to);
    bool rename_config(RowId id, QString configName);
    bool copy_config(RowId id, QString configName);

    tool::ex::Config *get_config(RowId id) const;
    tool::ex::Config *get_config(ConfigKey configKey) const ;
    QStringList get_configs_name() const;
    inline size_t get_configs_count() const noexcept {return configs.size();}

    inline QString to_string() const noexcept{return QSL("Component(") % m_name % QSL("|") % from_view(get_full_name(type)) % QSL("|") % QString::number(key()) % QSL(")");}

    constexpr int key() const noexcept{ return m_key();}
    constexpr ComponentKey c_key() const noexcept {return ComponentKey{key()};}

public:

    Type type = Type::SizeEnum;
    Category category   = Category::SizeEnum;
    std::unique_ptr<Config> initConfig = nullptr;

    RowId selectedConfigId = {0};
    std::vector<std::unique_ptr<Config>> configs = {};

private:

    IdKey m_key = IdKey(IdKey::Type::Component, -1);
    QString m_name = "default";    
};
}
