
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

#include "string_list_to_id_any_ndm.hpp"

using namespace tool::ex;

void StringListToIdAnyNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();

    // missing inputs
    if(check_if_missing_inputs(inputs)){
        return;
    }

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        auto strData = std::make_shared<StringData>(QSL("runtime"));
        strData->set_runtime();
        propagate_default_runtime({std::make_shared<IdAnyData>(IdAny{0, std::move(strData)})}, 0);
        return;
    }

    // cast
    auto data = dcast<StringListData>(inputs[0]);
    if(!data){
        set_invalid_cast();
        return;
    }

    // propagate
    QStringList txt = data->value();
    std::vector<std::shared_ptr<BaseNodeData>> outputs;
    for(int ii = 0; ii < txt.size(); ++ii){
        outputs.emplace_back(std::make_shared<IdAnyData>(IdAny{ii, std::make_shared<StringData>(txt[ii])}));
    }

    propagate_data(
        str::Convertor::to_str(QSL("Nb: ") % QString::number(txt.size())),
        std::move(outputs), 0
    );
}

void StringListToIdAnyNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}

#include "moc_string_list_to_id_any_ndm.cpp"
