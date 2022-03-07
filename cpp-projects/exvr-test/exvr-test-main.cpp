
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



// main.cpp
#define CATCH_CONFIG_RUNNER
#include <QCoreApplication>
#include "catch.hpp" // include after defining CATCH_CONFIG_RUNNER

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    const int res = Catch::Session().run(argc, argv);
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


