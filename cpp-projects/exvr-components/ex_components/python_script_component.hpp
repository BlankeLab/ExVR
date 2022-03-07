
/*******************************************************************************
** exvr-components                                                            **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

#pragma once

// std
#include <string>
#include <memory>

// boost
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

// base
#include "exvr/ex_component.hpp"


namespace tool::ex{

struct PyData{
    boost::python::object main;
    boost::python::object globals;
    boost::python::object module;
    boost::python::object component;

};

class PythonScriptComponent : public ExComponent{

public :

    bool initialize() override;
    void start_experiment() override;
    void start_routine() override;
    void set_visibility(bool visible) override;
    void set_update_state(bool doUpdate) override;
    void update()override;
    void play() override;
    void pause() override;
    void stop_routine()override;
    void stop_experiment() override;
    void clean() override;
    void slot(int index) override;

    std::string m_modulePath;
    std::string m_moduleName;
    std::string m_moduleDirectoryPath;
    std::string m_className;
    std::string m_pythonLibPath;

    static void signal_bool(int key, int index, bool value);
    static void signal_int(int key, int index, int value);
    static void signal_float(int key, int index, float value);
    static void signal_double(int key, int index, double value);
    static void signal_string(int key, int index, std::string value);

private :

    void remove_module();

    std::optional<boost::python::api::object> call_function(const std::string &funcName);

    template <typename T>
    std::optional<boost::python::api::object> call_function(const std::string &funcName, T arg);

    template <typename T1, typename T2>
    std::optional<boost::python::api::object> call_function(const std::string &funcName, T1 arg1, T2 arg2);

    std::unique_ptr<PyData> data = nullptr;
};
}
