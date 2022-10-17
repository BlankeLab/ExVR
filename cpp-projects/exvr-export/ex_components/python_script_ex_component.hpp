
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

class PythonScriptExComponent : public ExComponent{

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

    const char* componentStr = R"def(
import sys
import numpy as np

class Component(object):

    def initialize(self):
        ex.log("From python: initialize ")
        ex.log(sys.path[0])
        return True

    def start_experiment(self):
        ex.log("From python: start_experiment ")
        pass

    def stop_experiment(self):
        ex.log("From python: stop_experiment ")
        pass

    def start_routine(self):
        ex.log("From python: start_routine ")
        pass

    def stop_routine(self):
        ex.log("From python: stop_routine ")
        pass

    def update(self):
        pass

    def update_parameter_from_gui(self):
        pass

    def stop(self):
        pass

    def play(self):
        pass

    def pause(self):
        pass

    def set_visibility(self, visible):
        #ex.log("From python: set_visibility " + str(visible))
        pass

    def set_update_state(self, doUpdate):
        #ex.log("From python: set_update_state " + str(doUpdate))
        pass

    def clean(self):
        pass

)def";
};
}
