
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
#include <QString>
#include <QXmlStreamReader>
#include <QDebug>

// qt-utility
#include "qt_logger.hpp"

// local
#include "experiment/experiment.hpp"
#include "experiment/instance.hpp"

namespace tool::ex{

    class XmlIoManager;
    using XmlIoManagerUP = std::unique_ptr<XmlIoManager>;

    class XmlIoManager : public QObject{
        Q_OBJECT
    public :
        XmlIoManager(Experiment *experiment) : m_experiment(experiment){
        }


        void force_no_duration(){
            m_debugNoDuration = true;
        }
        void cancel_no_duration(){
            m_debugNoDuration = false;
        }

    private :

        bool check_start_node(QString nodeName);
        bool check_end_node(QString nodeName);

        // ### read attributes
        template<class T> std::optional<T> read_attribute(const QString &name, bool raiseError); // primary template

        bool has_attribute(const QStringList &names) const{
            for(const auto &name : names){                                
                if(r->attributes().hasAttribute(name)){
                    return true;
                }
            }
            return false;
        }

        template<class T> std::optional<T> read_attribute(const QStringList &names, bool raiseError){
            for(const auto &name : names){
                if(auto r = read_attribute<T>(name, false); r.has_value()){
                    return r;
                }
            }
            if(raiseError){
                QtLogger::error(QSL("[XML] No attribute found with names: ") % names.join(",") % QSL(", at line ") % QString::number(r->lineNumber()));
            }

            return {};
        }

        template<> std::optional<bool> read_attribute(const QString &name, bool raiseError){

            if(r->attributes().hasAttribute(name)){
                bool ok;
                auto value = r->attributes().value(name).toInt(&ok);
                if(ok){
                    return value == 1;
                }

                QtLogger::error(QSL("[XML] Invalid boolean attribute with name: ") %  name % QSL(" cannot convert from ") % r->attributes().value(name) % QSL(", at line ") % QString::number(r->lineNumber()));
                return {};
            }

            if(raiseError){
                QtLogger::error(QSL("[XML] Boolean attribute ") %  name % QSL(" doesn't exist, at line ") % QString::number(r->lineNumber()));
            }

            return {};
        }

        template<> std::optional<size_t> read_attribute(const QString &name, bool raiseError){
            if(auto v = read_attribute<int>(name, raiseError); v.has_value()){
                return {static_cast<size_t>(v.value())};
            }

            if(raiseError){
                QtLogger::error(QSL("[XML] Size_t attribute ") %  name % QSL(" doesn't exist, at line ") % QString::number(r->lineNumber()));
            }

            return {};
        }

        template<> std::optional<int> read_attribute(const QString &name, bool raiseError){

            if(r->attributes().hasAttribute(name)){
                bool ok;
                auto value = r->attributes().value(name).toInt(&ok);
                if(ok){
                    return value;
                }

                QtLogger::error(QSL("[XML] Invalid integer attribute [") % name % QSL("] cannot convert from ") % r->attributes().value(name) % QSL(", at line ") % QString::number(r->lineNumber()));
                return {};
            }

            if(raiseError){
                QtLogger::error(QSL("[XML] Integer attribute [") % name % QSL("] doesn't exist, at line ") % QString::number(r->lineNumber()));
            }

            return {};
        }


        template<> std::optional<double> read_attribute(const QString &name, bool raiseError){

            if(r->attributes().hasAttribute(name)){
                bool ok;
                auto value = r->attributes().value(name).toDouble(&ok);
                if(ok){
                    return value;
                }

                QtLogger::error(QSL("[XML] Invalid double attribute with name [") % name % QSL("] cannot convert from ") % r->attributes().value(name) % QSL(", at line ") % QString::number(r->lineNumber()));
                return {};
            }

            if(raiseError){
                QtLogger::error(QSL("[XML] Double attribute [") % name % QSL("], it doesn't exist, at line ") % QString::number(r->lineNumber()));
            }

            return {};
        }

        template<> std::optional<QString> read_attribute(const QString &name, bool raiseError){
            if(r->attributes().hasAttribute(name)){
                return r->attributes().value(name).toString().replace("\\n","\n");
            }

            if(raiseError){
                QtLogger::error(QSL("[XML] String attribute [") % name % QSL("], it doesn't exist, at line ") % QString::number(r->lineNumber()));
            }

            return {};
        }

        template <typename T>
        inline void assign_attribute(T &value, const QString &name, bool raiseError){
            if(auto v = read_attribute<T>(name, raiseError); v.has_value()){
                value = v.value();
                return;
            }
            if(raiseError){               
                QtLogger::error(QSL("[XML] Can't assign attribute [") % name % QSL("], it doesn't exist, at line ") % QString::number(r->lineNumber()));
            }
        }

        template <typename T>
        inline void assign_attribute(T &value, const QStringList &names, bool raiseError){
            if(auto v = read_attribute<T>(names, raiseError); v.has_value()){
                value = v.value();
                return;
            }
            if(raiseError){
                QtLogger::error(QSL("[XML] Can't assign attribute, it doesn't exist, at line ") % QString::number(r->lineNumber()));
            }
        }

        template <typename T>
        inline bool compare_attribute(const T &name, const T &target, bool raiseError){
            if(auto v = read_attribute<T>(name, raiseError); v.has_value()){
                return v.value() == target;
            }
            if(raiseError){
                QtLogger::error(QSL("[XML] Can't campare with attribute [") % name % QSL("], it doesn't exist, at line ") % QString::number(r->lineNumber()));
            }

            return false;
        }        


        // ### warnings
        QString invalid_bracket_error_message(int id, IdKey::Type type);

        // ### write
        void write_argument(const Arg &arg);
        void write_set(const Set *set);
        void write_generator(const Generator &generator);
        void write_config(const Config *config, bool initConfig = false);
        void write_interval(const Interval &interval);
        void write_timeline(const Timeline *timeline);
        void write_component(const Component *component);
        void write_components();
        void write_action(const Action *action);
        void write_connection(const Condition *condition, const Connection *connection);
        void write_condition(const Condition *condition);
        void write_connector(const Connector *connector);
        void write_element(const FlowElement *element);
        void write_loop(const Loop *loop);
        void write_isi(const Isi *isi);
        void write_routine(const Routine *routine);
        void write_settings();
        void write_resources();
        void write_flow_elements();
        void write_flow_sequence();
        void write_experiment();

        // ### read
        Generator           read_generator();
        std::unique_ptr<Config>            read_config();
        std::tuple<std::optional<Arg>, QString>  read_argument();
        std::optional<Interval> read_interval();
        std::unique_ptr<Timeline>          read_timeline();
        std::tuple<std::unique_ptr<Action>, QString> read_action();
        std::tuple<std::unique_ptr<Connection>, QString> read_connection(Condition *condition);
        std::tuple<std::unique_ptr<Connector>, QString> read_connector();
        std::unique_ptr<Condition>         read_condition(Routine *routine);
        std::unique_ptr<Component>         read_component();
        bool                read_components();
        std::unique_ptr<Resource> read_resource();

        std::unique_ptr<Routine> read_routine();
        std::tuple<std::unique_ptr<LoopNode>, std::unique_ptr<Loop>, std::unique_ptr<LoopNode>> read_loop();
        std::unique_ptr<Set> read_set();
        IsiUP               read_isi();
        std::vector<std::unique_ptr<Config>> read_configs();
        bool                read_ISIs();
        bool                read_loops();
        bool                read_routines();
        bool                read_flow_elements();
        bool                read_resources();
        bool                read_flow_sequence();
        bool                read_settings();
        bool                read_exp();
        std::unique_ptr<FlowElement>           read_element();

        void check_read_elements();


    private:

        bool m_debugNoDuration = false;
        Experiment *m_experiment = nullptr;
        std::unique_ptr<QXmlStreamReader> r = nullptr;
        std::unique_ptr<QXmlStreamWriter> w = nullptr;

        std::vector<std::unique_ptr<Routine>>  readXmlRoutines;
        std::vector<IsiUP>      readXmlISIs;
        std::vector<std::unique_ptr<LoopNode>> readXmlStartLoops;
        std::vector<std::unique_ptr<LoopNode>> readXmlEndLoops;

        QString expFileToLoad;
        QString expFileToSave;

    public slots:

        bool save_experiment();
        bool save_experiment_to_temp();
        bool save_experiment_file(QString expFilePath);
        bool save_experiment_as();

        bool load_experiment_file(QString xmlFilePath);
//        bool load_dropped_experiment_file(QString expFilePath);

        void open_temp_experiment_file();
        void open_temp_instance_file();
        void open_experiment_directory();
        void open_current_exp_launcher_log_file();
        void open_current_designer_log_file();
        void open_log_directory();

        void export_experiment_to();

        bool save_instance_to_temp(const Instance &instance);
        bool save_instance_file(const Instance &instance, QString instanceFilePath);
        std::unique_ptr<Instance> load_instance_file(QString instanceFilePath);


    };
}
