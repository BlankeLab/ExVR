
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

// local
#include "config_pw.hpp"

namespace tool::ex {

class ModelConfigParametersW : public ConfigParametersW{

public :

    ModelConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class CubeInitConfigParametersW : public ConfigParametersW{

public :

    CubeInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class PlaneInitConfigParametersW : public ConfigParametersW{

public :

    PlaneInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class CylinderInitConfigParametersW : public ConfigParametersW{

public :

    CylinderInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class SphereInitConfigParametersW : public ConfigParametersW{

public :

    SphereInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class TorusInitConfigParametersW : public ConfigParametersW{

public :

    TorusInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

class LinesInitConfigParametersW : public ConfigParametersW{
public :

    LinesInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class LinesConfigParametersW : public ConfigParametersW{

public :

    LinesConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


class LandmarkInitConfigParametersW : public ConfigParametersW{
public :

    LandmarkInitConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

class LandmarkConfigParametersW : public ConfigParametersW{

public :

    LandmarkConfigParametersW();

    void insert_widgets() override;
    void init_and_register_widgets() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};


}
