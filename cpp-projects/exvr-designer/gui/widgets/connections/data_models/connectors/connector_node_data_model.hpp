
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

// base
#include "utility/types.hpp"

// qt-utility
#include "gui/ex_widgets/ex_base_w.hpp"

// local
#include "gui/widgets/connections/data_models/base_node_data_model.hpp"
#include "gui/widgets/connections/data_models/base_embedded_widget.hpp"
#include "data/connector.hpp"


// nodes
using QtNodes::NodeDataType;
using QtNodes::PortType;
using QtNodes::PortIndex;

namespace tool::ex{

class ConnectorNodeDataModel : public BaseNodeDataModel{

    Q_OBJECT
public:

    ConnectorNodeDataModel(Connector::Type t, BaseNodeContainerW *w);
    virtual ~ConnectorNodeDataModel() override{}

    void initialize(QtNodes::NodeStyle style, ConnectorKey connectorKey);
    virtual void init_ports_caption();
    virtual void init_ports_types();
    virtual void init_ports_caption_visibility();

    virtual void update_from_connector(const Connector &connector);
    virtual void update_with_info(QStringView id, QStringView value);
    virtual Arg convert_to_arg() const;

    auto get_inputs() const{
        std::vector<std::shared_ptr<QtNodes::NodeData>> inputs(inputData.size());
        for(size_t ii = 0; ii < inputData.size(); ++ii){
            inputs[ii] = inputData[ii].lock();
        }
        return inputs;
    }

    // check
    // # circularity
    bool check_infinity_loop(bool unknowValue = true);

    // inputs
    bool has_inputs(const std_v1<std::shared_ptr<QtNodes::NodeData> > &inputs) const;
    // # no inputs
    bool check_if_no_inputs(const std_v1<std::shared_ptr<QtNodes::NodeData>> &inputs, std::optional<QString> text = {});
    // # missing inputs
    bool check_if_missing_inputs(const std_v1<std::shared_ptr<QtNodes::NodeData>> &inputs);
    // # runtime
    bool check_if_runtime_inputs(const std_v1<std::shared_ptr<QtNodes::NodeData>> &inputs);

    // data
    void propagate_default_runtime(std::vector<std::shared_ptr<BaseNodeData>> outputs, std::vector<size_t> indexes = {});
    void propagate_default_runtime(std::vector<std::shared_ptr<BaseNodeData>> outputs, size_t outputId);
    void propagate_data(QString valueStr, std::vector<std::shared_ptr<BaseNodeData>> outputs, std::vector<size_t> indexes = {});
    void propagate_data(QString valueStr, std::vector<std::shared_ptr<BaseNodeData>> outputs, size_t outputId);
    static std::shared_ptr<BaseNodeData> generate_default_runtime_any_data(){
        return std::make_shared<AnyData>(std::make_shared<IntData>(0));
    }
    void invalidate_data();
    void set_invalid_cast();

    // embedded widget
    // # text
    void set_embedded_widget_text(const QString &text);
//    void set_embedded_widget_icon(const QIcon &icon);
    void runtime_value_text();
    void unknow_value_text();
    void error_value_text();
    // # lock/unlock
    void lock_embedded_widget();
    void unlock_embedded_widget();
    // # widget
    BaseNodeContainerW *node_container()const{return m_widget;}
    QWidget *embeddedWidget() override {return m_widget;}

    virtual std::shared_ptr<NodeData> outData(PortIndex index) override;
    virtual void setInData(std::shared_ptr<NodeData> nodeData, PortIndex index) override;

    static bool is_runtime(const std::shared_ptr<NodeData> &data){
        return std::dynamic_pointer_cast<BaseNodeData>(data)->is_runtime();
    }

public slots:

    virtual void compute(){};

signals:

    void connector_modified_signal(ConnectorKey key, QString name, tool::ex::Arg arg);

protected:

    Connector::Type m_type;

    std_v1<std::weak_ptr<NodeData>> inputData;
    std_v1<std::shared_ptr<NodeData>> interData;
    std_v1<std::shared_ptr<NodeData>> outputData;

private:

    BaseNodeContainerW *m_widget = nullptr;    
    bool timerOn = false;
    int nbCalls = 0;
};

template <Connector::Type T, typename ExW>
class TypedConnectorDataModel : public ConnectorNodeDataModel{
public:

    TypedConnectorDataModel() : ConnectorNodeDataModel(T, new ExW()) {
        embedded_w()->initialize_container(caption(), Connector::get_widget_mode(T) == Connector::WidgetMode::Focus, Connector::get_text_format(T));
        embedded_w()->initialize();
    }

    ExW *embedded_w() const{
        return qobject_cast<ExW*>(node_container());
    }

    template<typename T2>
    static std::shared_ptr<T2> dcast(std::shared_ptr<NodeData> data){
        return std::dynamic_pointer_cast<T2>(data);
    }

    template<typename T1, typename T2>
    static T1 scast(T2 value){
        return  static_cast<T1>(value);
    }


};


}
