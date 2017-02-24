#include "catch.hpp"

#include <iostream>
#include <random>
#include <functional>
#include "number/Real.h"
using namespace Number;
using std::cout;
using std::endl;

static int real_test_num = 30;
static unsigned seed = 0;

unsigned genSeed(unsigned t) {
	unsigned a;
	if (t == 0)
		a = (unsigned)time(NULL);
	else
		a = t;
	std::cout << "Seed is:" << a << std::endl;
	return a;
}

Real RandomRealNumber() {
	static std::default_random_engine generator(genSeed(seed));
	static std::uniform_int_distribution<save_type> distribution, size_d(1, 1024), sig(0, 1);
	static auto dice = std::bind(distribution, generator);
	int size = size_d(generator);
	std::vector<save_type> vec(size);
	for (auto& num : vec) {
		num = dice();
	}
	if (vec.back() == 0)
		++vec.back();
	return Real(vec, sig(generator) ? 1 : -1, dice()%128);
}

TEST_CASE("Test initialize for Real", "[Initialize][Real]") {
	REQUIRE(Real() == 0);
	Real(1.0);
	Real((unsigned char)120);
	Real((unsigned int)3);
	Real((unsigned long)65536);
	Real((char)-105);
	Real((int)-236);
	Real((long)-657386);
	Real((float)3.1415926e10);
	Real((double)-2.718281828e6);
	Real((double)308273954e-200);
}

#define TEST_COM_BASE(TYPE, num1, num2, op, res) do{ \
	if(res) { \
		REQUIRE(TYPE(num1) op TYPE(num2)); \
		REQUIRE(TYPE(num1) op (num2)); \
		REQUIRE((num1) op TYPE(num2)); \
	} \
	else { \
		REQUIRE_FALSE(TYPE(num1) op TYPE(num2));\
		REQUIRE_FALSE((num1) op TYPE(num2));\
		REQUIRE_FALSE(TYPE(num1) op (num2));\
	}\
}while(0)

//num1 <= num2, eq = true iff. num1 == num2
template<typename Type, typename U, typename V>
void TestCompareBase2(U num1, V num2, bool eq) {
	TEST_COM_BASE(Type, num1, num2, == , eq);
	TEST_COM_BASE(Type, num1, num2, != , !eq);
	TEST_COM_BASE(Type, num2, num1, == , eq);
	TEST_COM_BASE(Type, num2, num1, != , !eq);
	TEST_COM_BASE(Type, num1, num2, <, !eq);
	TEST_COM_BASE(Type, num2, num1, <, false);
	TEST_COM_BASE(Type, num1, num2, >, false);
	TEST_COM_BASE(Type, num2, num1, >, !eq);
	TEST_COM_BASE(Type, num1, num2, <= , true);
	TEST_COM_BASE(Type, num2, num1, <= , eq);
	TEST_COM_BASE(Type, num1, num2, >= , eq);
	TEST_COM_BASE(Type, num2, num1, >= , true);
}

template<typename Type, typename U, typename V>
void TestCompareNumber(U num1, V num2) {
	if (num1 < num2)
		TestCompareBase2<Type>(num1, num2, false);
	else if (num1 > num2)
		TestCompareBase2<Type>(num2, num1, false);
	else
		TestCompareBase2<Type>(num1, num2, true);
}

TEST_CASE("Test compare operator for Real", "[Compare][Real]") {
	SECTION("Equal numbers.") {
		double test_number_1[] = { 0, 1, -1, 10086, -65536, 0xDF00, -0xDC00,
			0xDF00000, -0xDC00000, 1454767345, -1454767345, 0.0,
			271.8281828354, -65.336 };

		for (auto ele : test_number_1) {
			TestCompareNumber<Real>(ele, ele);
		}
	}

	SECTION("Nonequal numbers.") {
		std::pair<double, double> test_number_2[] = { { 0,1 },{ 0, -1 },
		{ -12323, 5 },{ 13579, 246810 },{ -13579, -2345 },{ 0xCF00, 0xDF00 },
		{ 467539025, 467539026 },{ -467539025, -467539026 },
		{1, 65372},
		{-106825736, -2567},
		{0.0, 3.14159265358979},
		{(float)27.1828354, (float)-27.1828354},
		{-65.336, (double)-30006587.2654},
		{(float)-23.3356, -30006587.2654}, };

		for (auto ele : test_number_2) {
			TestCompareNumber<Real>(ele.first, ele.second);
		}
	}
}

TEST_CASE("Test Parse and Tostring for Real.", "[ParseAndToString][Real]") {
	SECTION("Parse Right") {
		std::string test_number_1[] = { 
			"3.14159265358979323846264338327950288e101",
		"5",
		"0e14",
		"-0e0",
		"1e128", 
		"1.32468208213" };

		for (auto ele : test_number_1) {
			Real a, b;
			REQUIRE(b.Parse(ele.c_str()) == Number_Parse_OK);

			REQUIRE(a.Parse(b.ToString10().c_str()) == Number_Parse_OK);
			REQUIRE(a == b);
			REQUIRE(a.ToString10() == b.ToString10());
		}
	}

	SECTION("Parse wrong.") {
		std::string test_number_2[] = { "", "-", "0b1102",
			"0o123467758", "123743a", "0x23c4dfaeg", "0x-3df", "6.658765e"};

		for (auto ele : test_number_2) {
			Real a;
			REQUIRE(a.Parse(ele.c_str()) == Number_Parse_Failed);
			REQUIRE(a == 0);
		}
	}

	SECTION("Random number.") {
		for (int ii = 0;ii < real_test_num;++ii) {
			Real n1 = RandomRealNumber();
			Real n2;
			n2.SetPrecision(n1.GetPrecision());
			n2.Parse(n1.ToString10().c_str());
			REQUIRE(n1 == n2);
		}
	}
}

TEST_CASE("Test Add and Sub for Real", "[AddSub][Real]") {
	SECTION("Special number test.") {
		std::tuple<std::string, std::string, std::string> test_number[] = { { "0.0", "0.0", "0.0" },
		{ "-0.0", "0.0", "0.0" },{ "0", "1.3", "1.3" },{ "0", "-3", "-3" },{ "-7", "7", "0" },
		{ "32768", "65536", "98304" },{ "10000", "1000000", "1010000" } };

		for (auto ele : test_number) {
			Real s, a, b;
			REQUIRE(a.Parse(std::get<0>(ele).c_str()) == Number_Parse_OK);
			REQUIRE(b.Parse(std::get<1>(ele).c_str()) == Number_Parse_OK);
			REQUIRE(s.Parse(std::get<2>(ele).c_str()) == Number_Parse_OK);
			REQUIRE(a + b == s);
			REQUIRE(b + a == s);
			REQUIRE(s - a == b);
			REQUIRE(s - b == a);
			REQUIRE(a - s == -b);
			REQUIRE(b - s == -a);
		}
	}

	SECTION("Random number test.") {
		for (int ii = 0;ii < real_test_num;++ii) {
			Real a = RandomRealNumber();
			Real b = RandomRealNumber();
			Real r1 = a + b;
			Real r2 = r1 - a;
			REQUIRE((r2 - b).Abs() <= (0.000000000001 * r1.Abs()));
		}
	}
}

TEST_CASE("Test Multiply and Devide for Real", "[MulDev][Real]") {
	SECTION("Special number test.") {
		std::tuple<std::string, std::string, std::string> test_number[] = { { "0", "0", "0" },
		{ "-0", "0", "0" },
		{ "0", "1", "0" },
		{ "-3", "0", "0" },
		{ "2", "70", "140" },
		{ "-5", "30", "-150" },

		{ "338745", "24325", "8239972125" },
		{ "-60374", "22580", "-1363244920" },
		{ "-34750247679359595104195192713491853248905703459234", "-1",
			"34750247679359595104195192713491853248905703459234" },
		{ "34502938750234750245703459234857029503464564775029452934509287437676816581876214",
			"7490918801118394765103491091168307360192023441273509150198327409213539834673455",
			"258458712577969901107793595983076419519271833012883483684123496626476994193811141762278729300587831551290525355683871785611321045319590811232460657255921699370" },
		{ "2384750193485324891786231019822357117632491019432579320923759071264016501924345",
			"-9965977871263490182375019843750912381720472136487651923245912386419827364981234",
			"-23766367656766074498187360121055082313211874311420286616610754823487880627777351412999960762652087319961498758734391202707977726944188275025244657025812741730" }
		};

		for (auto ele : test_number) {
			Real s, a, b;
			s.SetPrecision(1024);
			b.SetPrecision(1024);
			a.SetPrecision(1024);
			REQUIRE(a.Parse(std::get<0>(ele).c_str()) == Number_Parse_OK);
			REQUIRE(b.Parse(std::get<1>(ele).c_str()) == Number_Parse_OK);
			REQUIRE(s.Parse(std::get<2>(ele).c_str()) == Number_Parse_OK);
			REQUIRE(a * b == s);
			REQUIRE(b * a == s);
			if (a != 0) {
				Real r1 = s / a;
				REQUIRE((r1 - b).Abs() <= (0.001 * b.Abs()));
			}
			if (b != 0) {
				Real r2 = s / b;
				REQUIRE((r2 - a).Abs() <= (0.001 * a.Abs()));
			}
		}
	}

	SECTION("Random number test.") {
		for (int ii = 0;ii < real_test_num;++ii) {
			Real a = RandomRealNumber() + 1.0;
			Real b = RandomRealNumber();
			Real r1 = a * b;
			Real r2 = r1 / a;
			REQUIRE((r2 - b).Abs() <= (0.001 * b.Abs()));
		}
	}
}
