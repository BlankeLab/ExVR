

/***********************************************************************************
** exvr-test                                                                      **
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
#include <format>

// catch
#include "catch.hpp"


// qt-utility
#include "qt_logger.hpp"

// exvr-designer
#include "IO/xml_io_manager.hpp"
#include "utility/path_utility.hpp"

using namespace tool;
using namespace tool::ex;



// https://www.froglogic.com/blog/tip-of-the-week/unit-tests-for-qt-based-applications-with-catch/

//    SECTION("Matrix 3x3"){
//        REQUIRE(geo::equals(dm1,from_glm(glmdm1)));
//        REQUIRE(dm1.determinant() == glm::determinant(glmdm1));
//    }

//    SECTION("Look at"){
//        auto lightView1 = from_glm(glm::lookAt(
//            glm::vec3(-2.0f, 4.0f, -1.0f),
//            glm::vec3( 0.0f, 0.0f,  0.0f),
//            glm::vec3( 0.0f, 1.0f,  0.0f)
//        ));

//        auto lightView2 = geo::Mat4f::LookAt(
//            geo::Pt3f{-2.0f, 4.0f, -1.0f},
//            geo::Vec3f{0.0f, 0.0f,  0.0f},
//            geo::Vec3f{0.0f, 1.0f,  0.0f}
//        );

//        REQUIRE(geo::equals(clean(lightView1),clean(lightView2)));
//    }

TEST_CASE("Experiment"){

    Experiment exp("1.0");
    auto types = Component::all_components_types();

    SECTION("Components types"){

        qDebug() << from_view(Component::get_type_name(types[0]));
        for(const auto & type : types){
            exp.add_new_component(type, {0});
        }
        REQUIRE(exp.compM.count() == types.size());
        qDebug() << "Components count: " << exp.compM.count();

        auto c = exp.compM.get_component(RowId{0});
        REQUIRE(!c->rename_config(RowId{1}, "test"));
        REQUIRE(c->rename_config(RowId{0}, "test"));
        c->add_config(std::make_unique<Config>("test2", ConfigKey{-1}));
        REQUIRE(!c->rename_config(RowId{0}, "test"));
    }

    SECTION("Routines"){

        exp.add_element(FlowElement::Type::Routine, 0);
        exp.add_element(FlowElement::Type::Routine, 2);
        exp.add_element(FlowElement::Type::Isi, 4);
        exp.add_element(FlowElement::Type::Routine, 6);
        exp.add_element(FlowElement::Type::Loop, 8);
        exp.add_element(FlowElement::Type::Routine, 10);

        REQUIRE(exp.get_routine(RowId{0})->key() == exp.elements[1]->key());
        REQUIRE(exp.get_routine(RowId{1})->key() == exp.elements[3]->key());
        REQUIRE(exp.get_routine(RowId{2})->key() == exp.elements[7]->key());
        REQUIRE(exp.get_routine(RowId{3})->key() == exp.elements[11]->key());

        auto names = exp.get_routines_name();
        REQUIRE(exp.get_routine(RowId{0})->name() == names[0]);
        REQUIRE(exp.get_routine(RowId{1})->name() == names[1]);
        REQUIRE(exp.get_routine(RowId{2})->name() == names[2]);
        REQUIRE(exp.get_routine(RowId{3})->name() == names[3]);
    }
}

TEST_CASE("Experiments loading"){

    return;

    std::vector<QString> failureExp={
        "","///","../../../..","\aàé_gub"
    };

    std::vector<QString> successExp={
        Paths::expDir % QSL("/examples/button_press_reaction_time_using_connections.xml"),
        Paths::expDir % QSL("/examples/camera_trajectories.xml"),
        Paths::expDir % QSL("/examples/image.xml"),
        Paths::expDir % QSL("/examples/input_controllers.xml"),
        Paths::expDir % QSL("/examples/keyboard_with_slider.xml"),
        Paths::expDir % QSL("/examples/logger_conditions_sample.xml"),
        Paths::expDir % QSL("/examples/read_write_udp.xml"),
        Paths::expDir % QSL("/examples/slider_moving_value.xml"),
        Paths::expDir % QSL("/examples/write_data_to_csv_using_connections.xml"),
        Paths::expDir % QSL("/examples/write_data_to_file_using_scripting.xml"),
        Paths::expDir % QSL("/debug/test_duplicated_connections.xml"),
    };

    tool::ex::Experiment exp("1.0a16");
    tool::ex::XmlIoManager xmlIoM(&exp);

    for(const auto &path : failureExp){
        QtLogger::message(QSL("Load exp from path: ") % path);
        REQUIRE(!xmlIoM.load_experiment_file(path));
        exp.clean_experiment();
    }

    for(const auto &path : successExp){
        QtLogger::message(QSL("Load exp from path: ") % path);
        REQUIRE(xmlIoM.load_experiment_file(path));
        exp.clean_experiment();
    }


}

