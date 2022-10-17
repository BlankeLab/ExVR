
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

// std
#include <ranges>

// Qt
#include <QApplication>
#include <QSplashScreen>

// base
#include "utility/format.hpp"

// local
#include "controller/exvr_controller.hpp"

using namespace tool::ex;

int main(int argc, char *argv[]){

    // build parameters
    const QString numVersion = "1.0a60";
    bool lncoComponents = true;

    // compiler check
    std::cout << tool::fmt("Start ExVR-designer v{}\n", numVersion.toStdString()); // c++20 check


    // doc generation
//    Component::Category c = Component::Category::Viewer;
//    for(auto t : Component::all_components_types()){
//        auto fn = Component::get_full_name(t);
//        auto un = Component::get_unity_name(t);

//        std::string_view cn = "-";
//        if(Component::get_category(t) != c){
//            c = Component::get_category(t);
//            cn = Component::to_string(c);
//        }
//        std::cout << std::format("|{}|{}|[link](components/{}_info.md)|[link](components/{}_csharp.md)|[link](components/{}_connections.md)|\n",
//            cn, fn, un, un, un
//        );
//    }

//    Connector::Category cc = Connector::Category::Resource;
//    for(auto t : Connector::all_types()){

//        auto n = Connector::get_caption(t);

//        std::string_view ccn = "-";
//        if(Connector::get_category(t) != cc){
//            cc  = Connector::get_category(t);
//            ccn = Connector::get_name(cc);
//        }

//        std::string_view cn = "-";
//        std::cout << std::format("|{}|{}|[link](connectors/{}_info.md)|[link](connectors/{}_connections.md)|\n",
//            ccn, n, ccn, ccn
//        );
//    }





    // build app
    QApplication a(argc, argv);

    QPixmap pixmap(":/splash/ex_vr_splash");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    splash.show();

    tool::ex::Paths::initialize_paths(QApplication::applicationDirPath());
    tool::ex::ExVrController controller(numVersion, lncoComponents);
    QCoreApplication::instance()->installEventFilter(&controller);

    QTimer::singleShot(1000, &splash, &QWidget::close);

    int ret = a.exec();
    std::cout << "Quit with: " << ret << "\n";
    return ret;
}


