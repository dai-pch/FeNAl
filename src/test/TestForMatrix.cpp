#include "catch.hpp"

#include "number/Matrix.h"
using namespace Number;

TEST_CASE("Test initialize of Matrix", "[Initialize][Matrix]") {
	std::vector<double> number(16);

	//REQUIRE(Integer() == 0);
    Matrix<int> a;
    Matrix<double> b(3, 3);
    Matrix<double> c(4, 4, number);
    REQUIRE(Matrix<int>(4, 4) == Matrix<int>(4, 4));
    
}

TEST_CASE("Test assign of Matrix", "[Assign][Matrix]") {
	std::vector<double> number{0,1,2,3,4,5,6,7,8};
	//REQUIRE(Integer() == 0);
    Matrix<double> a(3, 3, number);
    Matrix<int> b;
    b = a;
    Matrix<int> c(a);
}

TEST_CASE("Test equation of Matrix", "[Compare][Matrix]") {
	std::vector<double> number{0,1,2,3,4,5,6,7,8};
	//REQUIRE(Integer() == 0);
    REQUIRE(Matrix<int>(3, 3, number) == Matrix<int>(3, 3, number));
    REQUIRE(Matrix<int>(3, 3, number) != Matrix<int>(3, 3));
    REQUIRE(Matrix<int>(3, 3, number) != Matrix<int>(1, 9, number));
    REQUIRE(Matrix<int>(3, 3, number) != Matrix<int>(9, 1, number));
    REQUIRE(Matrix<int>(0, 3) == Matrix<int>(0, 0));
    REQUIRE(Matrix<int>(7, 0) != Matrix<int>(2, 4));
}

TEST_CASE("Test add or sub of Matrix", "[AddSub][Matrix]") {
	std::vector<double> number1{0,1,2,3,4,5,6,7,8};
    std::vector<double> number2{3,23,42,2,45,2,0,70,82};

    Matrix<double> a(3, 3, number1);
    Matrix<double> b(3, 3, number2);
    Matrix<double> sum = a + b;

	//REQUIRE(Integer() == 0);
    REQUIRE(a == sum - b);
    REQUIRE(sum - a == b);
}

TEST_CASE("Test multiply of Matrix", "[Multiply][Matrix]") {
	std::vector<double> number1{1, 2, 5, 9};
    std::vector<double> number2{3, 0, 2, 7};
    std::vector<double> res{7, 14, 33, 63};

    Matrix<double> a(2, 2, number1);
    Matrix<double> b(2, 2, number2);
    Matrix<double> c(2, 2, res);

	//REQUIRE(Integer() == 0);
    REQUIRE(c == a * b);
}

TEST_CASE("Test transpose of Matrix", "[Transpose][Matrix]") {
	std::vector<double> number1{1, 2, 5, 9};
    std::vector<double> number2{1, 5, 2, 9};

    Matrix<double> a(2, 2, number1);
    Matrix<double> b(2, 2, number2);

    REQUIRE(a.transpose() == b);
    REQUIRE(a == b.transpose());
    REQUIRE(a.transpose().transpose() == a);
}