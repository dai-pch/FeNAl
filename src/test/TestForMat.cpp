#include "catch.hpp"

#include "number/Mat.h"
using namespace Number;

TEST_CASE("Test initialize of Mat", "[Initialize][Mat]") {
	std::vector<double> number(16);
    std::vector<int> number2{1, 0, 0, 0, 1, 0, 0, 0, 1};

	//REQUIRE(Integer() == 0);
    Mat<int, 6, 6> a;
    Mat<double, 3, 3> b;
    Mat<double, 4, 4> c(number);
    Mat<int, 3, 3> d = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    REQUIRE((Mat<int, 4, 4>()) == (Mat<int, 4, 4>()));
    auto e = Mat<int, 3, 3>::ones();
    REQUIRE(d == (Mat<int, 3, 3>(number2)));
    REQUIRE(e == (Mat<int, 3, 3>(number2)));
    auto f = Mat<double, 5, 3>::ones();
    Vec<int, 3> g1 = {1, 0, 0};
    Vec<int, 3> g2 = {0, 1, 0};
    Vec<double, 3> g3 = {0.0, 0.0, 1.0};
    auto g = concat(g1, g2, g3);
    REQUIRE(g == (Mat<int, 3, 3>(number2)));
}

TEST_CASE("Test assign of Mat", "[Assign][Mat]") {
	std::vector<double> number{0,1,2,3,4,5,6,7,8};
	//REQUIRE(Integer() == 0);
    Mat<double, 3, 3> a(number);
    Mat<int, 3, 3> b;
    b = a;
    Mat<int, 3, 3> c(a);
}

TEST_CASE("Test equation of Mat", "[Compare][Mat]") {
	std::vector<double> number{0,1,2,3,4,5,6,7,8};
	//REQUIRE(Integer() == 0);
    REQUIRE((Mat<int, 3, 3>(number)) == (Mat<int, 3, 3>(number)));
    REQUIRE((Mat<int, 3, 3>(number)) != (Mat<int, 3, 3>()));
}

TEST_CASE("Test add or sub of Mat", "[AddSub][Mat]") {
	std::vector<double> number1{0,1,2,3,4,5,6,7,8};
    std::vector<double> number2{3,23,42,2,45,2,0,70,82};

    Mat<double, 3, 3> a(number1);
    Mat<double, 3, 3> b(number2);
    auto sum = a + b;

	//REQUIRE(Integer() == 0);
    REQUIRE(a == sum - b);
    REQUIRE(sum - a == b);
}

TEST_CASE("Test multiply of Mat", "[Multiply][Mat]") {
	std::vector<double> number1{1, 2, 5, 9};
    std::vector<double> number2{3, 0, 2, 7};
    std::vector<double> res{7, 14, 33, 63};

    Mat<double, 2, 2> a(number1);
    Mat<double, 2, 2> b(number2);
    Mat<double, 2, 2> c(res);

	//REQUIRE(Integer() == 0);
    REQUIRE(c == a * b);
}

TEST_CASE("Test transpose of Mat", "[Transpose][Mat]") {
	std::vector<double> number1{1, 2, 5, 9};
    std::vector<double> number2{1, 5, 2, 9};

    Mat<double, 2, 2> a(number1);
    Mat<double, 2, 2> b(number2);

    REQUIRE(a.transpose() == b);
    REQUIRE(a == b.transpose());
    REQUIRE(a.transpose().transpose() == a);
}

TEST_CASE("Test reverse of Mat", "[Reverse][Mat]") {
	std::vector<double> number1{1, 2, 5, 9};
    //std::vector<double> number1{1, 0, 0, 2};

    Mat<double, 2, 2> a(number1);
    Mat<double, 2, 2> b, c, d;

    b = a.reverse();
    c = b.reverse();
    d = a * b;

    REQUIRE((c - a).abs() < 0.001);
    REQUIRE(a * b == (Mat<double, 2, 2>::ones()));
    REQUIRE(b * a == (Mat<double, 2, 2>::ones()));
}