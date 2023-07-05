
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

// qt-utility
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class FlagPoleInitConfigParametersW : public ConfigParametersW{

public :

    FlagPoleInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class FlagPoleConfigParametersW : public ConfigParametersW{

public :

    FlagPoleConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

}
