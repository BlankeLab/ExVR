
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

#include "vector2_to_reals_ndm.hpp"

using namespace tool::ex;

void Vector2ToRealsNodeDataModel::compute(){

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
        propagate_default_runtime(
        {
            std::make_shared<RealData>(0.),
            std::make_shared<RealData>(0.)
        });
        return;
    }

    // cast
    auto data = dcast<Vector2Data>(inputs[0]);
    if(!data){
        set_invalid_cast();
        return;
    }

    // propagate
    geo::Vec2f vec = data->value();

    propagate_data(
    str::Convertor::to_str(vec),
    {
        std::make_shared<RealData>(vec.x()),
        std::make_shared<RealData>(vec.y())
    });
}

void Vector2ToRealsNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);

    inPortsInfo[0].caption = QSL("xy (") % get_name(io.inTypes[0]) % QSL(")");

    outPortsInfo[0].caption = QSL("x (") % get_name(io.outTypes[0]) % QSL(")");
    outPortsInfo[1].caption = QSL("y (") % get_name(io.outTypes[1]) % QSL(")");
}


#include "moc_vector2_to_reals_ndm.cpp"
