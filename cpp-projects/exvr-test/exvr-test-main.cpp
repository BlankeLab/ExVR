
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


//// catch
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"

//TEST_CASE( "1: All test cases reside in other .cpp files (empty)", "[multi-file:1]" ) {
//}

// Qt
#include <QCoreApplication>

// base
#include "utility/benchmark.hpp"

// qt-utility
#include "qt_logger.hpp"

// exvr-designer
#include "IO/xml_io_manager.hpp"
#include "utility/path_utility.hpp"

#define CATCH_CONFIG_RUNNER
#include "thirdparty/catch/catch.hpp"

using namespace tool;
using namespace tool::ex;

int main(int argc, char **argv){

//    tool::Bench::disable_display();

    QCoreApplication app(argc, argv);

    // init logging system
    QtLogger::init(QApplication::applicationDirPath() % QSL("/logs/"), QSL("designer_test.html"));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::normal,  QColor(189,189,189));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::warning, QColor(243, 158, 3));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::error,   QColor(244,4,4));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::unknow,  Qt::white);

    size_t countMessages = 0;
    size_t countWarnings = 0;
    size_t countErrors = 0;
    QtLogger::connect(QtLogger::get(), &QtLogger::message_signal, [&](QString m){++countMessages;
        qDebug() << m.remove("<p><font color=#bdbdbd>").remove("</font></p>\n");
    });
    QtLogger::connect(QtLogger::get(), &QtLogger::warning_signal, [&](QString w){++countWarnings;
        qWarning() << w.remove("<p><font color=#f39e03>").remove("</font></p>\n");
    });
    QtLogger::connect(QtLogger::get(), &QtLogger::error_signal, [&](QString e){++countErrors;
        qCritical() << e.remove("<p><font color=#f40404>").remove("</font></p>\n");
    });
    tool::ex::Paths::initialize_paths(QApplication::applicationDirPath() + "/../exvr-test");


    const int res = Catch::Session().run(argc, argv);

    QtLogger::message(QSL("Messages: ") % QString::number(countMessages));
    QtLogger::warning(QSL("Warnings: ") % QString::number(countWarnings));
    QtLogger::error(QSL("Errors: ") % QString::number(countErrors));

    return (res < 0xff ? res : 0xff);
}

// REQUIRE( expression ) : quit if failed
// CHECK( expression ) : continue if failed
// REQUIRE_FALSE( expression )
// CHECK_FALSE( expression )
// REQUIRE( performComputation() == Approx( 2.1 ) );
// using namespace Catch::literals;
// REQUIRE( performComputation() == 2.1_a );

// Approx target = Approx(100).epsilon(0.01);
// 100.0 == target; // Obviously true
// 200.0 == target; // Obviously still false
// 100.5 == target; // True, because we set target to allow up to 1% difference

// Approx target = Approx(100).margin(5);
// 100.0 == target; // Obviously true
// 200.0 == target; // Obviously still false
// 104.0 == target; // True, because we set target to allow absolute difference of at most 5

// REQUIRE_NOTHROW( expression ) and
// CHECK_NOTHROW( expression )
// REQUIRE_THROWS( expression ) and
// CHECK_THROWS( expression )
// REQUIRE_THROWS_WITH( expression, string or string matcher ) and
// CHECK_THROWS_WITH( expression, string or string matcher )
// REQUIRE_THROWS_MATCHES( expression, exception type, matcher for given exception type ) and
// CHECK_THROWS_MATCHES( expression, exception type, matcher for given exception type )
//REQUIRE_NOTHROW([&](){
//    int i = 1;
//    int j = 2;
//    auto k = i + j;
//    if (k == 3) {
//        throw 1;
//    }
//}());

// REQUIRE_THAT( lhs, matcher expression ) and
// CHECK_THAT( lhs, matcher expression )
// TEST_CASE_METHOD((Fixture<int, int>), "foo", "[bar]") {
//     SUCCEED();
// }


