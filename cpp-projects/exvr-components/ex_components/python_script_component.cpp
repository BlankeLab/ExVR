
/*******************************************************************************
** exvr-components                                                            **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

#include "python_script_component.hpp"

// std
#include <iostream>
#include <thread>

// boost
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// local
#include "utility/string.hpp"


using namespace tool::ex;


namespace bp = boost::python;


BOOST_PYTHON_MODULE(ex){

    bp::class_<std::vector<double>>("VectorDouble").def(bp::vector_indexing_suite<std::vector<double>>() );

    bp::def("log", &ExComponent::log_message, bp::arg("message"));
    bp::def("log_error", &ExComponent::log_error, bp::arg("error"));
    bp::def("signal_bool", &ExComponent::signal_bool, bp::arg("key"), bp::arg("index"), bp::arg("value"));
    bp::def("signal_int", &ExComponent::signal_int, bp::arg("key"), bp::arg("index"), bp::arg("value"));
    bp::def("signal_float", &ExComponent::signal_float, bp::arg("key"), bp::arg("index"), bp::arg("value"));
    bp::def("signal_double", &ExComponent::signal_double, bp::arg("key"), bp::arg("index"), bp::arg("value"));
    bp::def("signal_string", &ExComponent::signal_string, bp::arg("key"), bp::arg("index"), bp::arg("value"));
}

template <class T>
boost::python::list toPythonList(std::vector<T> vector) {
    typename std::vector<T>::iterator iter;
    boost::python::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return list;
}


std::string extract_exception(){

    PyObject *exc,*val,*tb;
    PyErr_Fetch(&exc,&val,&tb);
    PyErr_NormalizeException(&exc,&val,&tb);
    bp::handle<> hexc(exc),hval(bp::allow_null(val)),htb(bp::allow_null(tb));

    if(!hval){
        return bp::extract<std::string>(bp::str(hexc));
    }else{
        bp::object traceback(bp::import("traceback"));
        bp::object format_exception(traceback.attr("format_exception"));
        bp::object formatted_list(format_exception(hexc,hval,htb));
        bp::object formatted(bp::str("").join(formatted_list));
        return bp::extract<std::string>(formatted);
    }
}

std::optional<bp::api::object> PythonScriptComponent::call_function(const std::string &funcName){

    try{
        return data->component.attr(funcName.c_str())();
    }catch(const bp::error_already_set&){
        ExComponent::log_error(extract_exception());
    }
    return {};
}

template<typename T>
std::optional<bp::api::object> PythonScriptComponent::call_function(const std::string &funcName, T arg){

    try{
        return data->component.attr(funcName.c_str())(arg);
    }catch(const bp::error_already_set&){
        ExComponent::log_error(extract_exception());
    }
    return {};
}

template<typename T1, typename T2>
std::optional<bp::api::object> PythonScriptComponent::call_function(const std::string &funcName, T1 arg1, T2 arg2){

    try{
        return data->component.attr(funcName.c_str())(arg1, arg2);
    }catch(const bp::error_already_set&){
        ExComponent::log_error(extract_exception());
    }
    return {};
}


bool PythonScriptComponent::initialize(){

    bool loadModuleSuccess = false;
    try{
        if(!Py_IsInitialized()){
            // Append ex module;
            PyImport_AppendInittab("ex", &PyInit_ex);

            // Call to Py_Initialize
            Py_Initialize();
        }

        // Read data from initConfig
        m_modulePath = get<std::string>(ParametersContainer::InitConfig, "path_module");

        auto [moduleDirectoryPath, moduleName] = tool::str::split_path_and_filename(m_modulePath);
        tool::str::remove_from_right(moduleName, '.');
        m_moduleName = moduleName;
        m_moduleDirectoryPath = moduleDirectoryPath;
        m_className    = get<std::string>(ParametersContainer::InitConfig, "class_name");
        m_pythonLibPath= get<std::string>(ParametersContainer::InitConfig, "path_lib");

        // Add paths to sys.path        
        PyRun_SimpleString(componentStr); // TEST

        PyRun_SimpleString("import sys;");
        PyRun_SimpleString(std::string("sys.path.append(\"" + m_moduleDirectoryPath + "\");").c_str());
        PyRun_SimpleString(std::string("sys.path.append(\"" + m_pythonLibPath + "\");").c_str());

        // Init python objects
        data = std::make_unique<PyData>();

        // Import main
        data->main = bp::import("__main__");
        data->globals  = data->main.attr("__dict__");

        // Load module
        data->module = bp::import(m_moduleName.c_str());

        // Load class
        data->component = data->module.attr(m_className.c_str())();

        data->component.attr("key") = get<int>(ParametersContainer::Dynamic, "key");
        loadModuleSuccess = true;

    }catch(const bp::error_already_set&){
        ExComponent::log_error("Error during python module loading: " + extract_exception());
    }

    if(loadModuleSuccess){
        // Call initialization from python
        if(auto state = call_function("initialize"); state.has_value()){
            if(bp::extract<bool>(state.value())){
                return true;
            }
        }
    }

    // remove module
    try{
        remove_module();
    }catch(const bp::error_already_set&){
        ExComponent::log_error("Error during module removal: " + extract_exception());
    }

    return false;
}

void PythonScriptComponent::start_experiment() {
    call_function("start_experiment");
}

void PythonScriptComponent::stop_experiment() {
    call_function("stop_experiment");
}

void PythonScriptComponent::start_routine() {
    call_function("start_routine");
}

void PythonScriptComponent::stop_routine() {
    call_function("stop_routine");
}

void PythonScriptComponent::set_visibility(bool visible){
    call_function("set_visibility", visible);
}

void PythonScriptComponent::set_update_state(bool doUpdate){
    call_function("set_update_state", doUpdate);
}

void PythonScriptComponent::update() {
    call_function("update");
}

void PythonScriptComponent::play() {
    call_function("play");
}

void PythonScriptComponent::pause() {
    call_function("pause");
}

void PythonScriptComponent::clean() {
    call_function("clean");
    remove_module();
    // Do not call Py_Finalize() with Boost.Python.
}

void PythonScriptComponent::slot(int index){

    std::string valueName = std::string("slot") + std::to_string(index);
//    log("PyComponent::slot(int index) : " + valueName + " " + std::to_string(dynamic.count(valueName)));

//    if(contains(ParametersContainer::Dynamic, valueName)){
//        auto v = &containers[ParametersContainer::Dynamic][valueName];
//        if(v->type() == typeid (bool)){
//            call_function("slot", index, std::any_cast<bool>(*v));
//            return;
//        }else if(v->type() == typeid (int)){
//            call_function("slot", index, std::any_cast<int>(*v));
//            return;
//        }else if(v->type() == typeid (float)){
//            call_function("slot", index, std::any_cast<float>(*v));
//            return;
//        }else if(v->type() == typeid (double)){
//            call_function("slot", index, std::any_cast<double>(*v));
//            return;
//        }else if(v->type() == typeid (std::string)){
//            call_function("slot", index, std::any_cast<std::string>(*v));
//            return;
//        }
//    }


//    if(dynamicArray.count(valueName)){
//        auto &v =dynamicArray[valueName];
//        if(std::get<0>(v).type() == typeid(std::vector<double>)){

//            auto values = std::any_cast<std::vector<double>>(std::get<0>(v));
////            log_error("Values double " + std::to_string(values.size()));
//            call_function("slot", index, toPythonList(values));
//            return;
//        }
//    }
    log_error("No value with name " + valueName + " found.");
}

void PythonScriptComponent::remove_module(){
    PyRun_SimpleString(std::string("if '" + m_moduleName + "' in sys.modules:\n\tdel sys.modules[\"" + m_moduleName + "\"]").c_str());
    log_message("Module removed: " + m_moduleName);
}
