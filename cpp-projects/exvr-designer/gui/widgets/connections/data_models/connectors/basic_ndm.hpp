
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
#include <QLabel>

// nodes
#include "nodes/NodeDataModel.hpp"
#include "nodes/NodeData.hpp"
using QtNodes::PortType;

// qt-utility
#include "gui/ex_widgets/ex_label_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_notepad_w.hpp"

// local
#include "gui/ex_widgets/ex_config_w.hpp"
#include "gui/widgets/connections/data_models/connectors/connector_node_data_model.hpp"
#include "gui/widgets/connections/data_models/data/nodes_data.hpp"

namespace tool::ex {

class NextNodeDataModel : public TypedConnectorDataModel<Connector::Type::Next, PlaceHolderEmbeddedW>{
public:
    void init_ports_caption() override;
};

class PreviousNodeDataModel : public TypedConnectorDataModel<Connector::Type::Previous, PlaceHolderEmbeddedW>{
public:
    void init_ports_caption() override;
};

class StopNodeDataModel : public TypedConnectorDataModel<Connector::Type::Stop, PlaceHolderEmbeddedW>{
public:
    void init_ports_caption() override;
};

class PauseNodeDataModel : public TypedConnectorDataModel<Connector::Type::Pause, PlaceHolderEmbeddedW>{
public:
    void init_ports_caption() override;
};

class NextWithNameEmbeddedW : public NodeContainerW<ExLineEditW>{
Q_OBJECT
public:
    void initialize() override;
};
class NextWithNameNodeDataModel : public TypedConnectorDataModel<Connector::Type::Next_with_name, NextWithNameEmbeddedW>{
Q_OBJECT
public slots:
    void compute() override;
public:
    void init_ports_caption() override;
};

class PreviousWithNameEmbeddedW : public NodeContainerW<ExLineEditW>{
Q_OBJECT
public:
    void initialize() override;
};
class PreviousWithNameNodeDataModel : public TypedConnectorDataModel<Connector::Type::Previous_with_name, PreviousWithNameEmbeddedW>{
Q_OBJECT
public slots:
    void compute() override;
public:
    void init_ports_caption() override;
};

class NextWithCondEmbeddedW : public NodeContainerW<ExLineEditW>{
Q_OBJECT
public:
    void initialize() override;
};
class NextWithCondNodeDataModel : public TypedConnectorDataModel<Connector::Type::Next_with_cond, NextWithCondEmbeddedW>{
Q_OBJECT
public slots:
    void compute() override;    
public:
    void init_ports_caption() override;
};

class PreviousWithCondEmbeddedW : public NodeContainerW<ExLineEditW>{
Q_OBJECT
public:
    void initialize() override;
};
class PreviousWithCondNodeDataModel : public TypedConnectorDataModel<Connector::Type::Previous_with_cond, PreviousWithCondEmbeddedW>{
Q_OBJECT
public slots:
    void compute() override;
public:
    void init_ports_caption() override;
};

class ForceComponentConfigW  : public NodeContainerW<ExConfigW>{
    Q_OBJECT
public:
    void initialize() override;
};

class ForceComponentConfigNodeDataModel : public TypedConnectorDataModel<Connector::Type::Force_component_config, ForceComponentConfigW>{
    Q_OBJECT
public slots:
    void compute() override;
public:
    void init_ports_caption() override;
};


class PostItConfigW  : public NodeContainerW<ExNotepadW>{
    Q_OBJECT
 public:
    void initialize() override;
};

class PostItNodeDataModel : public TypedConnectorDataModel<Connector::Type::Post_it, PostItConfigW>{
    Q_OBJECT
public slots:
    void compute() override;
};


}


