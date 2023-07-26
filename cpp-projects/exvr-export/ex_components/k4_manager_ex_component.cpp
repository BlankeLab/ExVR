/***********************************************************************************
** exvr-export                                                                    **
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

#include "k4_manager_ex_component.hpp"

// std
#include <chrono>
#include <format>
#include <filesystem>
#include <execution>

// base
#include "utility/time.hpp"
// # network
#include "network/kinect4/k4_server_network.hpp"
#include "network/kinect4/k4_server_network_settings.hpp"
// # camera
#include "camera/kinect4/k4_server_data.hpp"
#include "camera/kinect4/k4_model.hpp"

using namespace std::chrono;

using namespace tool;
using namespace tool::ex;
using namespace tool::camera;
using namespace tool::geo;
using namespace tool::network;


struct GrabberSettings{
    bool connected = false;
    size_t id = 0;
    camera::K4Filters filters;
    camera::K4DeviceSettings device = camera::K4DeviceSettings::default_init_for_manager();
    camera::K4ColorSettings color;
    camera::K4Model model;
};

struct K4ManagerExComponent::Impl{

    K4ServerNetwork network;
    K4ServerData serverData;
    K4ServerNetworkSettings networkSettings;
    std::vector<GrabberSettings> grabbersS;

    std::mutex readMessagesM;
    std::deque<std::pair<size_t, network::K4Feedback>> messages;
    std::vector<std::pair<size_t, network::K4Feedback>> messagesR;

    std::atomic_int framesReceived = 0;

    std::vector<size_t> indices1D;

    auto set_connections() -> void{

        K4ServerConnection::feedback_signal.connect([&](size_t id, K4Feedback feedback){
            readMessagesM.lock();
            messages.emplace_back(id, feedback);
            readMessagesM.unlock();
        });

        K4ServerConnection::compressed_frame_signal.connect([&](size_t idCamera, std::shared_ptr<camera::K4CompressedFrame> cloudFrame){
            framesReceived++;
            serverData.new_compressed_frame(idCamera, cloudFrame);
        });
    }

    auto delete_connections(){
        K4ServerConnection::feedback_signal.disconnect_all();
        K4ServerConnection::compressed_frame_signal.disconnect_all();
    }

    auto connect_grabbers() -> void {
        for(const auto &grabberS : grabbersS){
            network.init_connection(grabberS.id);
        }
    }

    auto disconnect_grabbers() -> void {
        for(const auto &grabberS : grabbersS){
            network.send_command(grabberS.id, K4Command::Disconnect);
        }
    }

    auto quit_grabbers() -> void {
        for(const auto &grabberS : grabbersS){
            network.send_command(grabberS.id, K4Command::Quit);
        }
    }

    auto clean() -> void {
        network.clean();
        serverData.clean();
        grabbersS.clear();
    }
};

K4ManagerExComponent::K4ManagerExComponent() :  i(std::make_unique<Impl>()){
}

K4ManagerExComponent::~K4ManagerExComponent(){

}

auto K4ManagerExComponent::initialize() -> bool{

    Logger::set_logger_ptr(logger = exp->logger.get());

    log_message(std::format("Initialize K4ManagerExComponent with key {}.\n", key()));

    debugBypass = get<int>(ParametersContainer::InitConfig, "debug_bypass") == 1;

    // init network settings
    if(!i->networkSettings.initialize()){
        return false;
    }

    // init settings from files
    auto networkSettingsPath    = get<std::string>(ParametersContainer::Dynamic, "path_network_settings_file");
    auto deviceSettingsPath     = get<std::string>(ParametersContainer::Dynamic, "path_device_settings_file");
    auto filtersSettingsPath    = get<std::string>(ParametersContainer::Dynamic, "path_filters_file");
    auto colorSettingsPath      = get<std::string>(ParametersContainer::Dynamic, "path_color_settings_file");
    auto modelSettingsPath      = get<std::string>(ParametersContainer::Dynamic, "path_model_file");

    // read network settings file
    if(std::filesystem::exists(networkSettingsPath)){
        if(!i->networkSettings.init_from_file(networkSettingsPath)){
            return false;
        }
        set<int>(ParametersContainer::Dynamic, "network_settings_file_loaded",  1);
    }else{
        set<int>(ParametersContainer::Dynamic, "network_settings_file_loaded",  0);
        log_error("No network file found.\n");
        return false;
    }

    Logger::message("Network infos:\n");
    size_t id = 0;
    for(const auto &info : i->networkSettings.clientsInfo){
        Logger::message(std::format("ID:[{}] RI:[{}] RA:[{}] RP:[{}] SA:[{}] SP:[{}].\n",
            id, info.idReadingInterface, info.readingAdress, info.readingPort, info.sendingAdress, info.sendingPort));
        ++id;
    }

    auto nbConnections = i->networkSettings.clientsInfo.size();
    if(nbConnections == 0){
        log_error("No connection defined in network file.\n");
        return false;
    }
    i->grabbersS = {};
    i->grabbersS.resize(nbConnections);
    set<int>(ParametersContainer::Dynamic, "nb_connections", static_cast<int>(nbConnections));


    // read others settings files
    std::vector<files::BinaryFileSettings*> devicesS,colorS,filtersS;
    std::vector<files::TextSettings*> modelsS;
    size_t idG = 0;
    for(auto &grabberS : i->grabbersS){
        devicesS.push_back(&grabberS.device);
        colorS.push_back(&grabberS.color);
        filtersS.push_back(&grabberS.filters);
        modelsS.push_back(&grabberS.model);
        grabberS.id = idG++;
    }

    // # device
    if(get<int>(ParametersContainer::InitConfig, "device_init_file") == 1){
        if(std::filesystem::exists(deviceSettingsPath)){
            if(!files::BinaryFileSettings::init_from_file(devicesS, deviceSettingsPath)){
                Logger::error(std::format("Error while reading device settings file with path [{}].\n", deviceSettingsPath));
                return false;
            }
            set<int>(ParametersContainer::Dynamic, "device_settings_file_loaded",  1);
        }else{
            set<int>(ParametersContainer::Dynamic, "device_settings_file_loaded",  0);
            log_warning("No device settings file defined.\n");
        }
    }

    // # filters
    if(get<int>(ParametersContainer::InitConfig, "filters_init_file") == 1){
        if(std::filesystem::exists(filtersSettingsPath)){
            if(!files::BinaryFileSettings::init_from_file(filtersS, filtersSettingsPath)){
                Logger::error(std::format("Error while reading filters file with path [{}].\n", filtersSettingsPath));
                return false;
            }
            set<int>(ParametersContainer::Dynamic, "filters_file_loaded",  1);
        }else{
            set<int>(ParametersContainer::Dynamic, "filters_file_loaded",  0);
            log_warning("No filters file defined.\n");
        }
    }

    // # color
    if(get<int>(ParametersContainer::InitConfig, "color_init_file") == 1){
        if(std::filesystem::exists(colorSettingsPath)){
            if(!files::BinaryFileSettings::init_from_file(colorS, colorSettingsPath)){
                Logger::error(std::format("Error while reading color settings file with path [{}].\n", colorSettingsPath));
                return false;
            }
            set<int>(ParametersContainer::Dynamic, "color_settings_file_loaded",  1);
        }else{
            set<int>(ParametersContainer::Dynamic, "color_settings_file_loaded",  0);
            log_warning("No color settings file defined.\n");
        }
    }

    // # model
    if(std::filesystem::exists(modelSettingsPath)){
        if(!files::TextSettings::init_from_file(modelsS, modelSettingsPath)){
            Logger::error(std::format("Error while reading models file with path [{}].\n", modelSettingsPath));
            return false;
        }
        set<int>(ParametersContainer::Dynamic, "models_file_loaded",  1);
    }else{
        set<int>(ParametersContainer::Dynamic, "models_file_loaded",  0);
        log_warning("No models file defined.\n");
    }


    // transmit calibration matrices
    for(size_t ii = 0; ii < i->grabbersS.size(); ++ii){

        std::vector<float> m(16, 0.f);
        for(size_t jj = 0; jj < 16; ++jj){
            m[jj] = static_cast<float>(i->grabbersS[ii].model.transformation.array[jj]);
        }
        std::string name = "tr_" + std::to_string(ii);
        set_array(ParametersContainer::Dynamic, name, m);
    }

    if(debugBypass){
        return true;
    }

    if(!i->network.initialize(i->networkSettings.clientsInfo)){
        log_error("Cannot initialize network.\n");
        return false;
    }


    i->serverData.initialize(i->network.connections_nb());   

    return true;
}

auto K4ManagerExComponent::clean() -> void{

    if(debugBypass){
        return;
    }
    i->clean();
}

auto K4ManagerExComponent::start_experiment() -> void{

    if(debugBypass){
        return;
    }

    // connections
    i->set_connections();

    // set connections with grabbers
    i->connect_grabbers();


    startExperimentTime =  Time::nanoseconds_since_epoch();

}


auto K4ManagerExComponent::stop_experiment() -> void{

    if(debugBypass){
        return;
    }

    i->disconnect_grabbers();
    i->delete_connections();
}

auto K4ManagerExComponent::update_from_current_config() -> void{

    K4Delay delay;
    delay.delayMs = get<int>(ParametersContainer::Dynamic, "delay");
    for(size_t ii = 0; ii < i->grabbersS.size(); ++ii){
        i->network.send_delay(ii, delay);
    }
}

auto K4ManagerExComponent::update() -> void{

    if(debugBypass){
        return;
    }

    read_messages();

    set<int>(ParametersContainer::Dynamic, "nb_frames_received", i->framesReceived);

}

auto K4ManagerExComponent::read_messages() -> void{

    if(i->readMessagesM.try_lock()){

        auto size = i->messages.size();
        if(size != 0){
            for(size_t ii = 0; ii < size; ++ii){
                i->messagesR.push_back(i->messages.front());
                i->messages.pop_front();
            }
        }

        i->readMessagesM.unlock();
    }

    for(const auto &message : i->messagesR){
        if(message.second.receivedMessageType == MessageType::init_network_infos && message.second.feedback == FeedbackType::message_received){

            log_message(std::format("Grabber [{}] connected.\n", message.first));
            set<int>(
                ParametersContainer::Dynamic,
                std::format("grabber_connected_{}", message.first),
                i->network.get_connection(message.first)->grabber_connected_to_server() ? 1 : 0
            );

            if((get<int>(ParametersContainer::InitConfig, "device_init_file") == 1) || (get<int>(ParametersContainer::InitConfig, "device_init_default") == 1)){
                i->network.send_device_settings(message.first, i->grabbersS[message.first].device);
            }
            if((get<int>(ParametersContainer::InitConfig, "color_init_file") == 1) || (get<int>(ParametersContainer::InitConfig, "color_init_default") == 1)){
                i->network.send_color_settings(message.first, i->grabbersS[message.first].color);
            }
            if((get<int>(ParametersContainer::InitConfig, "filters_init_file") == 1) || (get<int>(ParametersContainer::InitConfig, "filters_init_default") == 1)){
                i->network.send_filters(message.first, i->grabbersS[message.first].filters);
            }
        }
    }
    i->messagesR.clear();
}

auto K4ManagerExComponent::get_cloud_frame_data(size_t idCamera, size_t currentFrameId, camera::K4VertexMeshData *vertices) -> std::tuple<bool, size_t, size_t>{

    if(auto frame = i->serverData.get_frame(idCamera); frame != nullptr){

        if(currentFrameId == frame->idCapture){
            return {false, frame->idCapture, frame->cloud.size()};
        }

//        if(i->indices1D.size() < frame->cloud.size()){
//            i->indices1D.resize(frame->cloud.size());
//            std::iota(std::begin(i->indices1D), std::end(i->indices1D), 0);
//        }

//        for_each(std::execution::par_unseq, std::begin(i->indices1D), std::end(i->indices1D), [&](size_t id){
//            vertices[id].pos = frame->cloud.vertices[id];
//            vertices[id].pos.x() *= -1.f;
//            vertices[id].col = geo::Pt4<std::uint8_t>{
//                static_cast<std::uint8_t>(frame->cloud.colors[id].x()*255.f),
//                static_cast<std::uint8_t>(frame->cloud.colors[id].y()*255.f),
//                static_cast<std::uint8_t>(frame->cloud.colors[id].z()*255.f),
//                255
//            };
//        });

        for(size_t id = 0; id < frame->cloud.size(); ++id){
            vertices[id].pos = frame->cloud.vertices[id];
//            vertices[id].pos.x() *= -1.f;
            vertices[id].col = geo::Pt4<std::uint8_t>{
                static_cast<std::uint8_t>(frame->cloud.colors[id].x()*255.f),
                static_cast<std::uint8_t>(frame->cloud.colors[id].y()*255.f),
                static_cast<std::uint8_t>(frame->cloud.colors[id].z()*255.f),
                255
            };
        }


        return {true, frame->idCapture, frame->cloud.size()};
    }
    return {false, 0, 0};
}

