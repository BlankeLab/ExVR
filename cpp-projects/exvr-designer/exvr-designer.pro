

# /***********************************************************************************
# ** exvr-designer                                                                  **
# ** MIT License                                                                    **
# ** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
# ** Permission is hereby granted, free of charge, to any person obtaining a copy   **
# ** of this software and associated documentation files (the "Software"), to deal  **
# ** in the Software without restriction, including without limitation the rights   **
# ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
# ** copies of the Software, and to permit persons to whom the Software is          **
# ** furnished to do so, subject to the following conditions:                       **
# **                                                                                **
# ** The above copyright notice and this permission notice shall be included in all **
# ** copies or substantial portions of the Software.                                **
# **                                                                                **
# ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
# ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
# ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
# ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
# ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
# ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
# ** SOFTWARE.                                                                      **
# ************************************************************************************/

####################################### repo
EXVR_REPOSITORY_DIR      = $$PWD"/../.."

####################################### PRI
# defines compiling options
include(../exvr-settings.pri)
# defines projects paths and variables
include(../exvr-projects.pri)
# defines thirdparty includes and libs
include(../exvr-thirdparty.pri)

####################################### TARGET
equals(CFG, "debug"){
    TARGET = ExVR-designerd
}
equals(CFG, "release"){
    TARGET = ExVR-designer
}

####################################### TEMPLATE
equals(EXVR_DESIGNER_TARGET, "app"){
    TEMPLATE = app
    CONFIG += precompile_header
    CONFIG += console
    # console
    DEFINES += QT_MESSAGELOGCONTEXT
    PRECOMPILED_HEADER = stable.h
}

####################################### BUILD FILES
OBJECTS_DIR = $$EXVR_DESIGNER_OBJ
MOC_DIR     = $$EXVR_DESIGNER_MOC
RCC_DIR     = $$EXVR_DESIGNER_RCC
UI_DIR      = $$EXVR_DESIGNER_UI
DESTDIR     = $$EXVR_DESIGNER_DEST
PRECOMPILED_DIR = $$EXVR_DESIGNER_OBJ

####################################### CONFIG
QT  += core gui network concurrent widgets opengl


####################################### INCLUDES

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = stable.h

INCLUDEPATH += \
    # base
    $$BASE_INCLUDES \
    # qt-utility
    $$QT_UTILITY_INCLUDES \
    # local
    $$EXVR_DESIGNER_MOC \
    # third-party
    $$QWT_INCLUDES \
    $$NODES_INCLUDES \
    $$MAGIC_ENUM_INCLUDES\
#    $$QMARKDOWN_TEXT_EDIT_INCLUDES\

####################################### LIBRAIRIES

LIBS +=  \
    # base
    $$BASE_LIB \
    # qt-utility
    $$QT_UTILITY_LIB \
    # third-party
    $$QWT_LIBS \
    $$NODES_LIB \
#    $$QMARKDOWN_TEXT_EDIT_LIBS\

####################################### PROJECT FILES


HEADERS += \
    # global
    forward.hpp \
    # controller
    controller/exvr_controller.hpp \
    # utility
    utility/path_utility.hpp \
    # IO
    IO/xml_io_manager.hpp \
    # settings
    settings/display.hpp \
    # launcher
    launcher/exp_launcher.hpp \
    launcher/exp_launcher_communication.hpp \
    launcher/exp_launcher_process.hpp \
    # data
    data/states.hpp \
    data/components_manager.hpp \
    data/gui.hpp \
    data/set.hpp \
    data/connection_node.hpp \
    data/connector.hpp \
    data/element.hpp \
    data/interval.hpp \
    data/routine.hpp \
    data/isi.hpp \
    data/loop.hpp \
    data/component.hpp \
    data/timeline.hpp \
    data/condition.hpp \
    data/config.hpp \
    data/settings.hpp \
    data/action.hpp \
    data/connection.hpp \
    data/node_flow.hpp \
    # resources
    resources/resource.hpp \
    resources/resources_manager.hpp \
    # flow
    flow/loop_flow_element.hpp \
    flow/node_flow_element.hpp \
    flow/routine_flow_element.hpp \
    flow/isi_flow_element.hpp \
    flow/flow_sequence.hpp \
    flow/add_flow_element.hpp \
    flow/remove_flow_element.hpp \
    flow/move_flow_element.hpp \
    flow/movable_flow_element.hpp \
    flow/flow_element.hpp \
    # experiment
    experiment/experiment.hpp \
    experiment/randomizer.hpp \
    experiment/instance.hpp \
    experiment/global_signals.hpp \
    # widgets
    ## main
    utility/script_utility.hpp \
    widgets/components/config_parameters/buttons_ui_pw.hpp \
    widgets/components/config_parameters/csharp_pw.hpp \
    widgets/components/config_parameters/fpp_camera_pw.hpp \
    widgets/components/config_parameters/serial_port_reader_pw.hpp \
    widgets/components/config_parameters/volumetric_video_pw.hpp \
    widgets/connections/data_models/connectors/resources_ndm.hpp \
    widgets/designer_window.hpp\
    ## elements
    widgets/dialogs/benchmark_dialog.hpp \
    widgets/dialogs/copy_to_conditions_dialog.hpp \
    widgets/elements/flow_diagram_widget.hpp \
    widgets/elements/element_viewer_widget.hpp \
    ## components
    widgets/components/component_config_dialog.hpp \
    widgets/components/components_manager_widget.hpp \
    widgets/components/component_widget.hpp \
    widgets/components/config_widget.hpp \
    ### parameters widgets
    widgets/components/config_parameters/humanoid_avatar_pw.hpp \
    widgets/components/config_parameters/fpp_avatar_camera_pw.hpp \
    widgets/components/config_parameters/tpp_avatar_camera_pw.hpp \
    widgets/components/config_parameters/flag_pole_pw.hpp \
    widgets/components/config_parameters/asset_bundle_pw.hpp \
    widgets/components/config_parameters/multi_ab_pw.hpp \
    widgets/components/config_parameters/biopac_pw.hpp \
    widgets/components/config_parameters/fop_robot_pw.hpp \
    widgets/components/config_parameters/blend_fade_viewer_pw.hpp \
    widgets/components/config_parameters/sub_parts_pw.hpp \
    widgets/components/config_parameters/config_pw.hpp \
    widgets/components/config_parameters/sky_pw.hpp \
    widgets/components/config_parameters/humanoid_controller_pw.hpp \
    widgets/components/config_parameters/text_viewer_pw.hpp \
    widgets/components/config_parameters/fixation_cross_viewer_pw.hpp \
    widgets/components/config_parameters/image_viewer_pw.hpp \
    widgets/components/config_parameters/webcam_viewer_pw.hpp \
    widgets/components/config_parameters/fov_simulator_pw.hpp \
    widgets/components/config_parameters/falling_spheres_pw.hpp \
    widgets/components/config_parameters/mirror_pw.hpp \
    widgets/components/config_parameters/flashing_dot_pw.hpp \
    widgets/components/config_parameters/mri_pw.hpp \
    widgets/components/config_parameters/camera_target_pw.hpp \
    widgets/components/config_parameters/camera_trajectory_pw.hpp \
    widgets/components/config_parameters/camera_trajectory_file_pw.hpp \
    widgets/components/config_parameters/camera_pw.hpp \
    widgets/components/config_parameters/python_script_pw.hpp \
    widgets/components/config_parameters/sonceboz_sg_pw.hpp \
    widgets/components/config_parameters/duo_tracking_pw.hpp \
    widgets/components/config_parameters/scene_scaner_pw.hpp \
    widgets/components/config_parameters/leap_motion_arms_display_pw.hpp \
    widgets/components/config_parameters/attach_object_to_hand_pw.hpp \
    widgets/components/config_parameters/kinect_body_tracking_pw.hpp \
    widgets/components/config_parameters/thera_trainer_platform_pw.hpp \
    widgets/components/config_parameters/thera_trainer_tracking_pw.hpp \
    widgets/components/config_parameters/kinect_manager_pw.hpp \
    widgets/components/config_parameters/qualisys_tracking_pw.hpp \
    widgets/components/config_parameters/cloud_pw.hpp \
    widgets/components/config_parameters/scaner_video_pw.hpp \
    widgets/components/config_parameters/microphone_pw.hpp \
    widgets/components/config_parameters/audio_source_pw.hpp \
    widgets/components/config_parameters/mouse_pw.hpp \
    widgets/components/config_parameters/joypad_pw.hpp \
    widgets/components/config_parameters/keyboard_pw.hpp \
    widgets/components/config_parameters/serial_port_writer_pw.hpp \
    widgets/components/config_parameters/udp_reader_pw.hpp \
    widgets/components/config_parameters/udp_writer_pw.hpp \
    widgets/components/config_parameters/parallel_port_writer_pw.hpp \
    widgets/components/config_parameters/logger_pw.hpp \
    widgets/components/config_parameters/text_resource_pw.hpp \
    widgets/components/config_parameters/image_resource_pw.hpp \
    widgets/components/config_parameters/plot_resource_pw.hpp \
    widgets/components/config_parameters/video_file_pw.hpp \
    widgets/components/config_parameters/webcam_pw.hpp \
    widgets/components/config_parameters/video_generator_pw.hpp \
    widgets/components/config_parameters/video_file_camera_pw.hpp \
    widgets/components/config_parameters/slider_ui_pw.hpp \
    widgets/components/config_parameters/primitives_model_pw.hpp \
    widgets/components/config_parameters/target_to_grab_pw.hpp \
    widgets/components/config_parameters/mark_to_clean_pw.hpp \
    #### settings
    ## ex_items
    ex_widgets/ex_camera_target_w.hpp \
    ex_widgets/ex_component_w.hpp \
    ex_widgets/ex_components_list_w.hpp \
    ex_widgets/ex_config_w.hpp \
    ex_widgets/ex_parameters_generator_w.hpp \
    ex_widgets/ex_resource_w.hpp \
    ex_widgets/ex_tab_w.hpp \
    ex_widgets/ex_time_source_w.hpp \
    ex_widgets/ex_resources_list_w.hpp \
    ex_widgets/generation/gen_ui_item_dialog.hpp \
    ## dialogs
    widgets/dialogs/generate_instances_dialog.hpp \
    widgets/dialogs/resources_manager_dialog.hpp \
    widgets/dialogs/documentation_dialog.hpp \
    widgets/dialogs/settings_dialog.hpp \
    ## routines
    widgets/routines/timeline_widget.hpp \
    widgets/routines/action_widget.hpp \
    widgets/routines/condition_widget.hpp \
    widgets/routines/routine_tab_widget.hpp \
    widgets/routines/routines_manager_tab_widget.hpp \
    ## connections
    widgets/connections/ex_flow_view_widget.hpp \
    widgets/connections/connections_widget.hpp \
    widgets/connections/data_models/base_embedded_widget.hpp \
    widgets/connections/data_models/base_node_data_model.hpp \
    widgets/connections/data_models/data/nodes_data_converters.hpp \
    widgets/connections/data_models/all_node_data_models.hpp \
    widgets/connections/data_models/data/nodes_data.hpp \
    widgets/connections/data_models/connectors/connector_node_data_model.hpp \
    widgets/connections/data_models/connectors/component_node_data_model.hpp \
    widgets/connections/data_models/connectors/transform_to_vectors_ndm.hpp \
    widgets/connections/data_models/connectors/vectors_to_transform_ndm.hpp \
    widgets/connections/data_models/connectors/reals_to_vector2_ndm.hpp \
    widgets/connections/data_models/connectors/reals_to_vector3_ndm.hpp \
    widgets/connections/data_models/connectors/vector2_to_reals_ndm.hpp \
    widgets/connections/data_models/connectors/vector3_to_reals_ndm.hpp \
    widgets/connections/data_models/connectors/string_list_to_id_any_ndm.hpp \
    widgets/connections/data_models/connectors/transform_ndm.hpp \
    widgets/connections/data_models/connectors/boolean_ndm.hpp \
    widgets/connections/data_models/connectors/id_any_ndm.hpp \
    widgets/connections/data_models/connectors/integer_ndm.hpp \
    widgets/connections/data_models/connectors/real_ndm.hpp \
    widgets/connections/data_models/connectors/string_ndm.hpp \
    widgets/connections/data_models/connectors/vector3_ndm.hpp \
    widgets/connections/data_models/connectors/string_any_ndm.hpp \
    widgets/connections/data_models/connectors/keyboard_ndm.hpp \
    widgets/connections/data_models/connectors/check_joypad_ndm.hpp \
    widgets/connections/data_models/connectors/check_mouse_ndm.hpp \
    widgets/connections/data_models/connectors/check_str_ndm.hpp \
    widgets/connections/data_models/connectors/pass_value_trigger_ndm.hpp \
    widgets/connections/data_models/connectors/pass_values_ndm.hpp \
    widgets/connections/data_models/connectors/conditional_trigger_ndm.hpp \
    widgets/connections/data_models/connectors/conditional_gate_ndm.hpp \
    widgets/connections/data_models/connectors/check_id_ndm.hpp \
    widgets/connections/data_models/connectors/string_operation_ndm.hpp \
    widgets/connections/data_models/connectors/decimal_operation_ndm.hpp \
    widgets/connections/data_models/connectors/binary_operation_ndm.hpp \
    widgets/connections/data_models/connectors/variable_delay_ndm.hpp \
    widgets/connections/data_models/connectors/decimal_trigo_ndm.hpp \
    widgets/connections/data_models/connectors/delay_ndm.hpp \
    widgets/connections/data_models/connectors/decimal_counter_ndm.hpp \
    widgets/connections/data_models/connectors/logger_ndm.hpp \
    widgets/connections/data_models/connectors/curve_x_ndm.hpp \
    widgets/connections/data_models/connectors/basic_ndm.hpp \
    widgets/connections/data_models/connectors/time_ndm.hpp \
    widgets/connections/data_models/connectors/flow_routine_ndm.hpp \

SOURCES += \
    # main
    designer_main.cpp \
    # settings
    settings/display.cpp \
    # IO
    IO/xml_io_manager.cpp \
    # data
    data/connector.cpp \
    data/isi.cpp \
    data/timeline.cpp \
    data/component.cpp \
    data/condition.cpp \
    data/action.cpp \
    data/interval.cpp \
    data/components_manager.cpp \
    data/config.cpp \
    data/loop.cpp \
    data/routine.cpp \
    # experiment
    experiment/instance.cpp \
    experiment/global_signals.cpp \
    experiment/experiment.cpp \
    # controller
    controller/exvr_controller.cpp \
    # launcher
    launcher/exp_launcher.cpp \
    launcher/exp_launcher_communication.cpp \
    launcher/exp_launcher_process.cpp \
    # flow
    flow/add_flow_element.cpp \
    flow/move_flow_element.cpp \
    flow/remove_flow_element.cpp \
    flow/loop_flow_element.cpp \
    flow/isi_flow_element.cpp \
    flow/node_flow_element.cpp \
    flow/flow_sequence.cpp \
    flow/routine_flow_element.cpp \
    flow/movable_flow_element.cpp \
    flow/flow_element.cpp \
    # utility
    utility/path_utility.cpp \
    # resources
    resources/resources_manager.cpp \
    ## ex_widgets
    ex_widgets/generation/gen_ui_item_dialog.cpp \
    ex_widgets/ex_camera_target_w.cpp \
    ex_widgets/ex_component_w.cpp \
    ex_widgets/ex_components_list_w.cpp \
    ex_widgets/ex_config_w.cpp \
    ex_widgets/ex_parameters_generator_w.cpp \
    ex_widgets/ex_resource_w.cpp \
    ex_widgets/ex_time_source_w.cpp \
    ex_widgets/ex_tab_w.cpp \
    ex_widgets/ex_resources_list_w.cpp \
    # widgets
    utility/script_utility.cpp \
    widgets/components/config_parameters/biopac_pw.cpp \
    widgets/components/config_parameters/buttons_ui_pw.cpp \
    widgets/components/config_parameters/csharp_pw.cpp \
    widgets/components/config_parameters/fpp_camera_pw.cpp \
    widgets/components/config_parameters/parallel_port_writer_pw.cpp \
    widgets/components/config_parameters/serial_port_reader_pw.cpp \
    widgets/components/config_parameters/serial_port_writer_pw.cpp \
    widgets/components/config_parameters/volumetric_video_pw.cpp \
    widgets/connections/data_models/connectors/resources_ndm.cpp \
    widgets/designer_window.cpp \
    ## components
    widgets/components/config_widget.cpp \
    ### parameters widgets
    widgets/components/config_parameters/config_pw.cpp \
    widgets/components/config_parameters/camera_target_pw.cpp \
    widgets/components/config_parameters/camera_trajectory_pw.cpp \
    widgets/components/config_parameters/sub_parts_pw.cpp \
    widgets/components/config_parameters/humanoid_avatar_pw.cpp \
    widgets/components/config_parameters/fpp_avatar_camera_pw.cpp \
    widgets/components/config_parameters/tpp_avatar_camera_pw.cpp \
    widgets/components/config_parameters/keyboard_pw.cpp \
    widgets/components/config_parameters/logger_pw.cpp \
    widgets/components/config_parameters/asset_bundle_pw.cpp \
    widgets/components/config_parameters/multi_ab_pw.cpp \
    widgets/components/config_parameters/blend_fade_viewer_pw.cpp \
    widgets/components/config_parameters/plot_resource_pw.cpp \
    widgets/components/config_parameters/image_resource_pw.cpp \
    widgets/components/config_parameters/audio_source_pw.cpp \
    widgets/components/config_parameters/microphone_pw.cpp \
    widgets/components/config_parameters/humanoid_controller_pw.cpp \
    widgets/components/config_parameters/camera_pw.cpp \
    widgets/components/config_parameters/kinect_manager_pw.cpp \
    widgets/components/config_parameters/qualisys_tracking_pw.cpp \
    widgets/components/config_parameters/scene_scaner_pw.cpp \
    widgets/components/config_parameters/thera_trainer_platform_pw.cpp \
    widgets/components/config_parameters/sonceboz_sg_pw.cpp \
    widgets/components/config_parameters/sky_pw.cpp \
    widgets/components/config_parameters/image_viewer_pw.cpp \
    widgets/components/config_parameters/text_viewer_pw.cpp \
    widgets/components/config_parameters/fixation_cross_viewer_pw.cpp \
    widgets/components/config_parameters/webcam_viewer_pw.cpp \
    widgets/components/config_parameters/fov_simulator_pw.cpp \
    widgets/components/config_parameters/joypad_pw.cpp \
    widgets/components/config_parameters/mri_pw.cpp \
    widgets/components/config_parameters/python_script_pw.cpp \
    widgets/components/config_parameters/slider_ui_pw.cpp \
    ## dialogs
    widgets/dialogs/benchmark_dialog.cpp \
    widgets/dialogs/copy_to_conditions_dialog.cpp \
    widgets/dialogs/generate_instances_dialog.cpp \
    widgets/dialogs/resources_manager_dialog.cpp \
    widgets/dialogs/documentation_dialog.cpp \
    ## elements
    widgets/elements/flow_diagram_widget.cpp \
    widgets/elements/element_viewer_widget.cpp \
    widgets/components/component_widget.cpp \
    widgets/components/components_manager_widget.cpp \
    widgets/components/component_config_dialog.cpp \
    widgets/routines/timeline_widget.cpp \
    widgets/routines/action_widget.cpp \
    widgets/routines/condition_widget.cpp \
    widgets/routines/routine_tab_widget.cpp \
    widgets/routines/routines_manager_tab_widget.cpp \
    ## connections
    widgets/connections/connections_widget.cpp \
    widgets/connections/ex_flow_view_widget.cpp \
    widgets/connections/data_models/all_node_data_models.cpp \
    widgets/connections/data_models/base_embedded_widget.cpp \
    widgets/connections/data_models/data/nodes_data_converters.cpp \
    widgets/connections/data_models/base_node_data_model.cpp \
    widgets/connections/data_models/connectors/connector_node_data_model.cpp \
    widgets/connections/data_models/connectors/component_node_data_model.cpp \
    widgets/connections/data_models/connectors/transform_to_vectors_ndm.cpp \
    widgets/connections/data_models/connectors/vectors_to_transform_ndm.cpp \
    widgets/connections/data_models/connectors/reals_to_vector2_ndm.cpp \
    widgets/connections/data_models/connectors/reals_to_vector3_ndm.cpp \
    widgets/connections/data_models/connectors/vector2_to_reals_ndm.cpp \
    widgets/connections/data_models/connectors/vector3_to_reals_ndm.cpp \
    widgets/connections/data_models/connectors/string_list_to_id_any_ndm.cpp \
    widgets/connections/data_models/connectors/transform_ndm.cpp \
    widgets/connections/data_models/connectors/boolean_ndm.cpp \
    widgets/connections/data_models/connectors/id_any_ndm.cpp \
    widgets/connections/data_models/connectors/integer_ndm.cpp \
    widgets/connections/data_models/connectors/real_ndm.cpp \
    widgets/connections/data_models/connectors/string_ndm.cpp \
    widgets/connections/data_models/connectors/vector3_ndm.cpp \
    widgets/connections/data_models/connectors/string_any_ndm.cpp \
    widgets/connections/data_models/connectors/keyboard_ndm.cpp \
    widgets/connections/data_models/connectors/check_joypad_ndm.cpp \
    widgets/connections/data_models/connectors/check_mouse_ndm.cpp \
    widgets/connections/data_models/connectors/check_str_ndm.cpp \
    widgets/connections/data_models/connectors/pass_value_trigger_ndm.cpp \
    widgets/connections/data_models/connectors/pass_values_ndm.cpp \
    widgets/connections/data_models/connectors/conditional_trigger_ndm.cpp \
    widgets/connections/data_models/connectors/conditional_gate_ndm.cpp \
    widgets/connections/data_models/connectors/check_id_ndm.cpp \
    widgets/connections/data_models/connectors/string_operation_ndm.cpp \
    widgets/connections/data_models/connectors/decimal_operation_ndm.cpp \
    widgets/connections/data_models/connectors/binary_operation_ndm.cpp \
    widgets/connections/data_models/connectors/variable_delay_ndm.cpp \
    widgets/connections/data_models/connectors/decimal_trigo_ndm.cpp \
    widgets/connections/data_models/connectors/delay_ndm.cpp \
    widgets/connections/data_models/connectors/decimal_counter_ndm.cpp \
    widgets/connections/data_models/connectors/logger_ndm.cpp \
    widgets/connections/data_models/connectors/curve_x_ndm.cpp \
    widgets/connections/data_models/connectors/basic_ndm.cpp \
    widgets/connections/data_models/connectors/time_ndm.cpp \
    widgets/connections/data_models/connectors/flow_routine_ndm.cpp \

FORMS += \
    # elements
    forms/about_dialog.ui \
    forms/copy_to_conditions.ui \
    forms/routine.ui \
    forms/isi.ui \
    forms/loop.ui \
    # timelines
    forms/action.ui \
    forms/condition.ui \
    # dialogs
    forms/generate_instances_dialog.ui \
    # main
    forms/designer_main_window.ui \
    # unity
    forms/base_component_config_widget.ui \
    forms/settings.ui \
    forms/resources_manager_dialog.ui \
    forms/resource_type.ui

RESOURCES += \
    $$EXVR_CPP_RESOURCES_DIR"/resources.qrc" \


RC_ICONS = ExVR_logo.ico
