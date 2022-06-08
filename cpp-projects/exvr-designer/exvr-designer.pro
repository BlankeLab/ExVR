

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
    # controller
    controller/exvr_controller.hpp \
    # utility
    gui/dialogs/import_sub_exp_dialog.hpp \
    gui/dialogs/keys_dialog.hpp \
    gui/objects/add_button_object.hpp \
    gui/objects/move_button_object.hpp \
    gui/objects/remove_button_object.hpp \
    gui/widgets/components/config_parameters/environment_pw.hpp \
    gui/widgets/connections/data_models/base_node_data_model.hpp \
    gui/widgets/connections/data_models/connectors/from_time_any_ndm.hpp \
    utility/script_utility.hpp \
    utility/path_utility.hpp \
    # IO
    IO/xml_io_manager.hpp \
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
    data/interval.hpp \
    data/component.hpp \
    data/timeline.hpp \
    data/condition.hpp \
    data/config.hpp \
    data/settings.hpp \
    data/action.hpp \
    data/connection.hpp \
    ## flow_elements
    data/flow_elements/flow_element.hpp \
    data/flow_elements/routine.hpp \
    data/flow_elements/isi.hpp \
    data/flow_elements/loop.hpp \
    data/flow_elements/node_flow.hpp \
    # resources
    resources/resource.hpp \
    resources/resources_manager.hpp \
    # experiment
    experiment/experiment.hpp \
    experiment/randomizer.hpp \
    experiment/instance.hpp \
    experiment/global_signals.hpp \
    # gui
    ## settings
    gui/settings/display.hpp \
    ## objects
    gui/objects/flow_element_object.hpp \
    gui/objects/flow_sequence_object.hpp \
    gui/objects/isi_flow_element_object.hpp \
    gui/objects/loop_flow_element_object.hpp \
    gui/objects/movable_flow_element_object.hpp \
    gui/objects/node_flow_element_object.hpp \
    gui/objects/routine_flow_element_object.hpp \
    ## dialogs
    gui/dialogs/add_component_to_conditions_dialog.hpp \
    gui/dialogs/benchmark_dialog.hpp \
    gui/dialogs/copy_to_conditions_dialog.hpp \
    gui/dialogs/generate_instances_dialog.hpp \
    gui/dialogs/resources_manager_dialog.hpp \
    gui/dialogs/documentation_dialog.hpp \
    gui/dialogs/settings_dialog.hpp \
    ## widgets
    ### main
    gui/widgets/designer_window.hpp\
    ### elements
    gui/widgets/elements/flow_diagram_widget.hpp \
    gui/widgets/elements/element_viewer_widget.hpp \
    ### components
    gui/widgets/components/component_config_dialog.hpp \
    gui/widgets/components/components_manager_widget.hpp \
    gui/widgets/components/component_widget.hpp \
    gui/widgets/components/config_widget.hpp \
    #### parameters widgets
    gui/widgets/components/config_parameters/buttons_ui_pw.hpp \
    gui/widgets/components/config_parameters/csharp_pw.hpp \
    gui/widgets/components/config_parameters/fpp_camera_pw.hpp \
    gui/widgets/components/config_parameters/serial_port_reader_pw.hpp \
    gui/widgets/components/config_parameters/volumetric_video_pw.hpp \
    gui/widgets/components/config_parameters/humanoid_avatar_pw.hpp \
    gui/widgets/components/config_parameters/fpp_avatar_camera_pw.hpp \
    gui/widgets/components/config_parameters/tpp_avatar_camera_pw.hpp \
    gui/widgets/components/config_parameters/flag_pole_pw.hpp \
    gui/widgets/components/config_parameters/asset_bundle_pw.hpp \
    gui/widgets/components/config_parameters/multi_ab_pw.hpp \
    gui/widgets/components/config_parameters/biopac_pw.hpp \
    gui/widgets/components/config_parameters/fop_robot_pw.hpp \
    gui/widgets/components/config_parameters/blend_fade_viewer_pw.hpp \
    gui/widgets/components/config_parameters/sub_parts_pw.hpp \
    gui/widgets/components/config_parameters/config_pw.hpp \
    gui/widgets/components/config_parameters/humanoid_controller_pw.hpp \
    gui/widgets/components/config_parameters/text_viewer_pw.hpp \
    gui/widgets/components/config_parameters/fixation_cross_viewer_pw.hpp \
    gui/widgets/components/config_parameters/image_viewer_pw.hpp \
    gui/widgets/components/config_parameters/webcam_viewer_pw.hpp \
    gui/widgets/components/config_parameters/fov_simulator_pw.hpp \
    gui/widgets/components/config_parameters/falling_spheres_pw.hpp \
    gui/widgets/components/config_parameters/mirror_pw.hpp \
    gui/widgets/components/config_parameters/flashing_dot_pw.hpp \
    gui/widgets/components/config_parameters/mri_pw.hpp \
    gui/widgets/components/config_parameters/camera_target_pw.hpp \
    gui/widgets/components/config_parameters/camera_trajectory_pw.hpp \
    gui/widgets/components/config_parameters/camera_trajectory_file_pw.hpp \
    gui/widgets/components/config_parameters/camera_pw.hpp \
    gui/widgets/components/config_parameters/python_script_pw.hpp \
    gui/widgets/components/config_parameters/sonceboz_sg_pw.hpp \
    gui/widgets/components/config_parameters/duo_tracking_pw.hpp \
    gui/widgets/components/config_parameters/scene_scaner_pw.hpp \
    gui/widgets/components/config_parameters/leap_motion_arms_display_pw.hpp \
    gui/widgets/components/config_parameters/attach_object_to_hand_pw.hpp \
    gui/widgets/components/config_parameters/kinect_body_tracking_pw.hpp \
    gui/widgets/components/config_parameters/thera_trainer_platform_pw.hpp \
    gui/widgets/components/config_parameters/thera_trainer_tracking_pw.hpp \
    gui/widgets/components/config_parameters/kinect_manager_pw.hpp \
    gui/widgets/components/config_parameters/qualisys_tracking_pw.hpp \
    gui/widgets/components/config_parameters/cloud_pw.hpp \
    gui/widgets/components/config_parameters/scaner_video_pw.hpp \
    gui/widgets/components/config_parameters/microphone_pw.hpp \
    gui/widgets/components/config_parameters/audio_source_pw.hpp \
    gui/widgets/components/config_parameters/mouse_pw.hpp \
    gui/widgets/components/config_parameters/joypad_pw.hpp \
    gui/widgets/components/config_parameters/keyboard_pw.hpp \
    gui/widgets/components/config_parameters/serial_port_writer_pw.hpp \
    gui/widgets/components/config_parameters/udp_reader_pw.hpp \
    gui/widgets/components/config_parameters/udp_writer_pw.hpp \
    gui/widgets/components/config_parameters/parallel_port_writer_pw.hpp \
    gui/widgets/components/config_parameters/logger_pw.hpp \
    gui/widgets/components/config_parameters/text_resource_pw.hpp \
    gui/widgets/components/config_parameters/image_resource_pw.hpp \
    gui/widgets/components/config_parameters/plot_resource_pw.hpp \
    gui/widgets/components/config_parameters/video_file_pw.hpp \
    gui/widgets/components/config_parameters/webcam_pw.hpp \
    gui/widgets/components/config_parameters/video_generator_pw.hpp \
    gui/widgets/components/config_parameters/video_file_camera_pw.hpp \
    gui/widgets/components/config_parameters/slider_ui_pw.hpp \
    gui/widgets/components/config_parameters/primitives_model_pw.hpp \
    gui/widgets/components/config_parameters/target_to_grab_pw.hpp \
    gui/widgets/components/config_parameters/mark_to_clean_pw.hpp \
    ### ex_items
    gui/ex_widgets/ex_camera_target_w.hpp \
    gui/ex_widgets/ex_component_w.hpp \
    gui/ex_widgets/ex_components_list_w.hpp \
    gui/ex_widgets/ex_config_w.hpp \
    gui/ex_widgets/ex_parameters_generator_w.hpp \
    gui/ex_widgets/ex_resource_w.hpp \
    gui/ex_widgets/ex_tab_w.hpp \
    gui/ex_widgets/ex_time_source_w.hpp \
    gui/ex_widgets/ex_resources_list_w.hpp \
    gui/ex_widgets/generation/gen_ui_item_dialog.hpp \
    ### routines
    gui/widgets/routines/timeline_widget.hpp \
    gui/widgets/routines/action_widget.hpp \
    gui/widgets/routines/condition_widget.hpp \
    gui/widgets/routines/routine_tab_widget.hpp \
    gui/widgets/routines/routines_manager_tab_widget.hpp \
    ### connections
    gui/widgets/connections/ex_flow_view_widget.hpp \
    gui/widgets/connections/connections_widget.hpp \
    gui/widgets/connections/data_models/connectors/resources_ndm.hpp \
    gui/widgets/connections/data_models/base_embedded_widget.hpp \
    gui/widgets/connections/data_models/data/nodes_data_converters.hpp \
    gui/widgets/connections/data_models/all_node_data_models.hpp \
    gui/widgets/connections/data_models/data/nodes_data.hpp \
    gui/widgets/connections/data_models/connectors/connector_node_data_model.hpp \
    gui/widgets/connections/data_models/connectors/component_node_data_model.hpp \
    gui/widgets/connections/data_models/connectors/transform_to_vectors_ndm.hpp \
    gui/widgets/connections/data_models/connectors/vectors_to_transform_ndm.hpp \
    gui/widgets/connections/data_models/connectors/reals_to_vector2_ndm.hpp \
    gui/widgets/connections/data_models/connectors/reals_to_vector3_ndm.hpp \
    gui/widgets/connections/data_models/connectors/vector2_to_reals_ndm.hpp \
    gui/widgets/connections/data_models/connectors/vector3_to_reals_ndm.hpp \
    gui/widgets/connections/data_models/connectors/string_list_to_id_any_ndm.hpp \
    gui/widgets/connections/data_models/connectors/transform_ndm.hpp \
    gui/widgets/connections/data_models/connectors/boolean_ndm.hpp \
    gui/widgets/connections/data_models/connectors/id_any_ndm.hpp \
    gui/widgets/connections/data_models/connectors/integer_ndm.hpp \
    gui/widgets/connections/data_models/connectors/real_ndm.hpp \
    gui/widgets/connections/data_models/connectors/string_ndm.hpp \
    gui/widgets/connections/data_models/connectors/vector3_ndm.hpp \
    gui/widgets/connections/data_models/connectors/string_any_ndm.hpp \
    gui/widgets/connections/data_models/connectors/keyboard_ndm.hpp \
    gui/widgets/connections/data_models/connectors/check_joypad_ndm.hpp \
    gui/widgets/connections/data_models/connectors/check_mouse_ndm.hpp \
    gui/widgets/connections/data_models/connectors/check_str_ndm.hpp \
    gui/widgets/connections/data_models/connectors/pass_value_trigger_ndm.hpp \
    gui/widgets/connections/data_models/connectors/pass_values_ndm.hpp \
    gui/widgets/connections/data_models/connectors/conditional_trigger_ndm.hpp \
    gui/widgets/connections/data_models/connectors/conditional_gate_ndm.hpp \
    gui/widgets/connections/data_models/connectors/check_id_ndm.hpp \
    gui/widgets/connections/data_models/connectors/string_operation_ndm.hpp \
    gui/widgets/connections/data_models/connectors/decimal_operation_ndm.hpp \
    gui/widgets/connections/data_models/connectors/binary_operation_ndm.hpp \
    gui/widgets/connections/data_models/connectors/variable_delay_ndm.hpp \
    gui/widgets/connections/data_models/connectors/decimal_trigo_ndm.hpp \
    gui/widgets/connections/data_models/connectors/delay_ndm.hpp \
    gui/widgets/connections/data_models/connectors/decimal_counter_ndm.hpp \
    gui/widgets/connections/data_models/connectors/logger_ndm.hpp \
    gui/widgets/connections/data_models/connectors/curve_x_ndm.hpp \
    gui/widgets/connections/data_models/connectors/basic_ndm.hpp \
    gui/widgets/connections/data_models/connectors/time_ndm.hpp \
    gui/widgets/connections/data_models/connectors/flow_routine_ndm.hpp \

SOURCES += \
    # main
    designer_main.cpp \
    # IO
    IO/xml_io_manager.cpp \
    # data
    data/connector.cpp \
    data/timeline.cpp \
    data/component.cpp \
    data/condition.cpp \
    data/action.cpp \
    data/interval.cpp \
    data/components_manager.cpp \
    data/config.cpp \
    ## flow_elements
    data/flow_elements/loop.cpp \
    data/flow_elements/routine.cpp \
    data/flow_elements/isi.cpp \
    # experiment
    experiment/instance.cpp \
    experiment/global_signals.cpp \
    experiment/experiment.cpp \
    # controller
    controller/exvr_controller.cpp \
    # launcher
    experiment/randomizer.cpp \
    gui/dialogs/import_sub_exp_dialog.cpp \
    gui/dialogs/keys_dialog.cpp \
    gui/objects/add_button_object.cpp \
    gui/objects/move_button_object.cpp \
    gui/objects/remove_button_object.cpp \
    gui/widgets/components/config_parameters/environment_pw.cpp \
    gui/widgets/connections/data_models/connectors/from_time_any_ndm.cpp \
    launcher/exp_launcher.cpp \
    launcher/exp_launcher_communication.cpp \
    launcher/exp_launcher_process.cpp \
    # utility
    utility/path_utility.cpp \
    utility/script_utility.cpp \
    # resources
    resources/resources_manager.cpp \
    # gui
    ## settings
    gui/settings/display.cpp \
    ## objects
    gui/objects/flow_element_object.cpp \
    gui/objects/flow_sequence_object.cpp \
    gui/objects/isi_flow_element_object.cpp \
    gui/objects/loop_flow_element_object.cpp \
    gui/objects/movable_flow_element_object.cpp \
    gui/objects/node_flow_element_object.cpp \
    gui/objects/routine_flow_element_object.cpp \
    ## dialogs
    gui/dialogs/add_component_to_conditions_dialog.cpp \
    gui/dialogs/benchmark_dialog.cpp \
    gui/dialogs/copy_to_conditions_dialog.cpp \
    gui/dialogs/generate_instances_dialog.cpp \
    gui/dialogs/resources_manager_dialog.cpp \
    gui/dialogs/documentation_dialog.cpp \
    ## ex_widgets
    gui/ex_widgets/generation/gen_ui_item_dialog.cpp \
    gui/ex_widgets/ex_camera_target_w.cpp \
    gui/ex_widgets/ex_component_w.cpp \
    gui/ex_widgets/ex_components_list_w.cpp \
    gui/ex_widgets/ex_config_w.cpp \
    gui/ex_widgets/ex_parameters_generator_w.cpp \
    gui/ex_widgets/ex_resource_w.cpp \
    gui/ex_widgets/ex_time_source_w.cpp \
    gui/ex_widgets/ex_tab_w.cpp \
    gui/ex_widgets/ex_resources_list_w.cpp \
    ## widgets
    gui/widgets/designer_window.cpp \
    ### components
    gui/widgets/components/config_widget.cpp \
    gui/widgets/components/component_widget.cpp \
    gui/widgets/components/components_manager_widget.cpp \
    gui/widgets/components/component_config_dialog.cpp \
    #### parameters widgets
    gui/widgets/components/config_parameters/biopac_pw.cpp \
    gui/widgets/components/config_parameters/buttons_ui_pw.cpp \
    gui/widgets/components/config_parameters/csharp_pw.cpp \
    gui/widgets/components/config_parameters/fpp_camera_pw.cpp \
    gui/widgets/components/config_parameters/parallel_port_writer_pw.cpp \
    gui/widgets/components/config_parameters/serial_port_reader_pw.cpp \
    gui/widgets/components/config_parameters/serial_port_writer_pw.cpp \
    gui/widgets/components/config_parameters/volumetric_video_pw.cpp \
    gui/widgets/components/config_parameters/config_pw.cpp \
    gui/widgets/components/config_parameters/camera_target_pw.cpp \
    gui/widgets/components/config_parameters/camera_trajectory_pw.cpp \
    gui/widgets/components/config_parameters/sub_parts_pw.cpp \
    gui/widgets/components/config_parameters/humanoid_avatar_pw.cpp \
    gui/widgets/components/config_parameters/fpp_avatar_camera_pw.cpp \
    gui/widgets/components/config_parameters/tpp_avatar_camera_pw.cpp \
    gui/widgets/components/config_parameters/keyboard_pw.cpp \
    gui/widgets/components/config_parameters/logger_pw.cpp \
    gui/widgets/components/config_parameters/asset_bundle_pw.cpp \
    gui/widgets/components/config_parameters/multi_ab_pw.cpp \
    gui/widgets/components/config_parameters/blend_fade_viewer_pw.cpp \
    gui/widgets/components/config_parameters/plot_resource_pw.cpp \
    gui/widgets/components/config_parameters/image_resource_pw.cpp \
    gui/widgets/components/config_parameters/audio_source_pw.cpp \
    gui/widgets/components/config_parameters/microphone_pw.cpp \
    gui/widgets/components/config_parameters/humanoid_controller_pw.cpp \
    gui/widgets/components/config_parameters/camera_pw.cpp \
    gui/widgets/components/config_parameters/kinect_manager_pw.cpp \
    gui/widgets/components/config_parameters/qualisys_tracking_pw.cpp \
    gui/widgets/components/config_parameters/scene_scaner_pw.cpp \
    gui/widgets/components/config_parameters/thera_trainer_platform_pw.cpp \
    gui/widgets/components/config_parameters/sonceboz_sg_pw.cpp \
    gui/widgets/components/config_parameters/image_viewer_pw.cpp \
    gui/widgets/components/config_parameters/text_viewer_pw.cpp \
    gui/widgets/components/config_parameters/fixation_cross_viewer_pw.cpp \
    gui/widgets/components/config_parameters/webcam_viewer_pw.cpp \
    gui/widgets/components/config_parameters/fov_simulator_pw.cpp \
    gui/widgets/components/config_parameters/joypad_pw.cpp \
    gui/widgets/components/config_parameters/mri_pw.cpp \
    gui/widgets/components/config_parameters/python_script_pw.cpp \
    gui/widgets/components/config_parameters/slider_ui_pw.cpp \
    ### elements
    gui/widgets/elements/flow_diagram_widget.cpp \
    gui/widgets/elements/element_viewer_widget.cpp \
    gui/widgets/routines/timeline_widget.cpp \
    gui/widgets/routines/action_widget.cpp \
    gui/widgets/routines/condition_widget.cpp \
    gui/widgets/routines/routine_tab_widget.cpp \
    gui/widgets/routines/routines_manager_tab_widget.cpp \
    ### connections
    gui/widgets/connections/data_models/connectors/resources_ndm.cpp \
    gui/widgets/connections/connections_widget.cpp \
    gui/widgets/connections/ex_flow_view_widget.cpp \
    gui/widgets/connections/data_models/all_node_data_models.cpp \
    gui/widgets/connections/data_models/base_embedded_widget.cpp \
    gui/widgets/connections/data_models/data/nodes_data_converters.cpp \
    gui/widgets/connections/data_models/connectors/connector_node_data_model.cpp \
    gui/widgets/connections/data_models/connectors/component_node_data_model.cpp \
    gui/widgets/connections/data_models/connectors/transform_to_vectors_ndm.cpp \
    gui/widgets/connections/data_models/connectors/vectors_to_transform_ndm.cpp \
    gui/widgets/connections/data_models/connectors/reals_to_vector2_ndm.cpp \
    gui/widgets/connections/data_models/connectors/reals_to_vector3_ndm.cpp \
    gui/widgets/connections/data_models/connectors/vector2_to_reals_ndm.cpp \
    gui/widgets/connections/data_models/connectors/vector3_to_reals_ndm.cpp \
    gui/widgets/connections/data_models/connectors/string_list_to_id_any_ndm.cpp \
    gui/widgets/connections/data_models/connectors/transform_ndm.cpp \
    gui/widgets/connections/data_models/connectors/boolean_ndm.cpp \
    gui/widgets/connections/data_models/connectors/id_any_ndm.cpp \
    gui/widgets/connections/data_models/connectors/integer_ndm.cpp \
    gui/widgets/connections/data_models/connectors/real_ndm.cpp \
    gui/widgets/connections/data_models/connectors/string_ndm.cpp \
    gui/widgets/connections/data_models/connectors/vector3_ndm.cpp \
    gui/widgets/connections/data_models/connectors/string_any_ndm.cpp \
    gui/widgets/connections/data_models/connectors/keyboard_ndm.cpp \
    gui/widgets/connections/data_models/connectors/check_joypad_ndm.cpp \
    gui/widgets/connections/data_models/connectors/check_mouse_ndm.cpp \
    gui/widgets/connections/data_models/connectors/check_str_ndm.cpp \
    gui/widgets/connections/data_models/connectors/pass_value_trigger_ndm.cpp \
    gui/widgets/connections/data_models/connectors/pass_values_ndm.cpp \
    gui/widgets/connections/data_models/connectors/conditional_trigger_ndm.cpp \
    gui/widgets/connections/data_models/connectors/conditional_gate_ndm.cpp \
    gui/widgets/connections/data_models/connectors/check_id_ndm.cpp \
    gui/widgets/connections/data_models/connectors/string_operation_ndm.cpp \
    gui/widgets/connections/data_models/connectors/decimal_operation_ndm.cpp \
    gui/widgets/connections/data_models/connectors/binary_operation_ndm.cpp \
    gui/widgets/connections/data_models/connectors/variable_delay_ndm.cpp \
    gui/widgets/connections/data_models/connectors/decimal_trigo_ndm.cpp \
    gui/widgets/connections/data_models/connectors/delay_ndm.cpp \
    gui/widgets/connections/data_models/connectors/decimal_counter_ndm.cpp \
    gui/widgets/connections/data_models/connectors/logger_ndm.cpp \
    gui/widgets/connections/data_models/connectors/curve_x_ndm.cpp \
    gui/widgets/connections/data_models/connectors/basic_ndm.cpp \
    gui/widgets/connections/data_models/connectors/time_ndm.cpp \
    gui/widgets/connections/data_models/connectors/flow_routine_ndm.cpp \

FORMS += \
    # elements
    gui/forms/about_dialog.ui \
    gui/forms/conditions_selecter.ui \
    gui/forms/copy_to_conditions.ui \
    gui/forms/routine.ui \
    gui/forms/isi.ui \
    gui/forms/loop.ui \
    # timelines
    gui/forms/action.ui \
    gui/forms/condition.ui \
    # dialogs
    gui/forms/generate_instances_dialog.ui \
    # main
    gui/forms/designer_main_window.ui \
    # unity
    gui/forms/base_component_config_widget.ui \
    gui/forms/settings.ui \
    gui/forms/resources_manager_dialog.ui \
    gui/forms/resource_type.ui

RESOURCES += \
    $$EXVR_CPP_RESOURCES_DIR"/resources.qrc" \


RC_ICONS = ExVR_logo.ico
