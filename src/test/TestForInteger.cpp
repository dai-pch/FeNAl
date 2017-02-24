#include "catch.hpp"

#include <tuple>
#include <random>
#include <functional>
#include "number/Integer.h"
using namespace Number;

static int integer_test_num = 100;

TEST_CASE("Test initialize of Integer", "[Initialize][Integer]") {
	long long test_number[] = { 0, 1, 2, 10, 256, 65536, 0x60000000, 0x800700060005000, 0xff0fff0fff0fff0 };

	REQUIRE(Integer() == 0);
	
	for (auto ele : test_number) {
		Integer((char)ele);
		Integer((unsigned char)ele);
		Integer((short)ele);
		Integer((unsigned short)ele);
		Integer((int)ele);
		Integer((unsigned int)ele);
		Integer((long)ele);
		Integer((unsigned long)ele);
		Integer((long long)ele);
		Integer((unsigned long long)ele);
	}
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

TEST_CASE("Test compare operator for Integer", "[Compare][Integer]") {
	SECTION("Equal numbers.") {
		long long test_number_1[] = { 0, 1, -1, 10086, -65536, 0xDF00, -0xDC00,
			0xDF0000000000, -0xDC0000000000, 1234547576752345345, -1234547576752345345 };

		for (auto ele : test_number_1) {
			TestCompareNumber<Integer>(ele, ele);
		}
	}

	SECTION("Nonequal numbers.") {
		std::pair<long long, long long> test_number_2[] = { {0,1},{0, -1},
		{-12323, 5}, {13579, 246810}, {-13579, -2345}, {0xCF00, 0xDF00},
		{-0xDC00, -0xAE00}, {-0xDC00, 0xA0}, {0xCF0000000000, 0xDF0000000000},
		{-0xDC0000000000, -0xAE0000000000}, {-0xDC0000000000, 0xA000000000},
		{1234547576752345345, 1234547576752345346}, {-1234547576752345345, -1234547576752345346} };

		for (auto ele : test_number_2) {
			TestCompareNumber<Integer>(ele.first, ele.second);
		}
	}
}

TEST_CASE("Test Parse and Tostring for Integer.", "[ParseAndToString][Integer]") {
	SECTION("Parse Right") {
		std::pair<long long, std::string> test_number_1[] = {
		{0b0, "0b0"},
		{0b0, "-0b0"},
		{0b0, "+0b0"},
		{0b0, "0B0"},
		{0b110, "0b00000110"},
		{0b11010010, "0b11010010"},
		{-0b10001101, "-0b10001101"},
		{0b110100101010, "0b110100101010"},
		{-0b100011011010, "-0b100011011010"},
		{0b10001100111011110111001101100101, "0B10001100111011110111001101100101"},
		{0b0100110011101111011100110110010110001100111011110111001101100101,
			"0b0100110011101111011100110110010110001100111011110111001101100101"},

		//8
		{00, "0o0"},
		{01, "0O1"},
		{-03, "-0o3"},
		{05, "+0o5"},
		{073, "0o000073"},
		{0123045676, "0o123045676"},
		{-0775310246, "-0O775310246"},
		{012345673452671535530, "0o12345673452671535530"},
		{-012345673452671537530, "-0o12345673452671537530"},

		//10
		{0, "0"},
		{1, "1"},
		{2, "+2"},
		{-9, "-9"},
		{300753992, "300753992"},
		{-10010, "-10010"},
		{10086, "+10086"},
		{2423569876418239237, "2423569876418239237"},
		{-2423569876418239237, "-2423569876418239237"},

		//16
		{0x0, "0x0"},
		{0x1, "0x1"},
		{0x5, "+0x5"},
		{-0x8, "-0x8"},
		{-0x7, "-0X7"},
		{0xB0A8, "0x00000B0A8"},
		{0xA0B0CCDD00F0, "0XA0B0CCDD00F0"},
		{-0xE00BCCDD00F0, "-0XE00BCCDD00F0"},
		{0x123456789ABCDEFD, "0x123456789abcdefd"},
		{-0x123456789ABCDEFC, "-0x123456789abcdefc"},
		{0xAA00CCDD0000000, "0XAA00CCDD0000000"},
		{0xAA00CCDD0000000, "0xAA00ccdd0000000"},

		{100000000000000000, "100000000000000000"},
		{ 0, "-0" },
		{ 65536327680 ,"65536327680" },
		{ 2789432110987654321 ,"2789432110987654321" },
		{ -1278943211098765432 , "-1278943211098765432" },
		{ 0x10, "0x10" },
		{ -0xCC, "-0xCC" },
		{ 0xABCDEF ,"0xabcdef" },
		{ 0X1237aaef ,"0X1237aaef" },
		{ 0x123456789abcde ,"0x123456789abcde"},
		{0xaacd ,"0x00000aacd"} };
		
		for (auto ele : test_number_1){
			Integer a(ele.first), b;
			REQUIRE(b.Parse(ele.second) == Number_Parse_OK);
			REQUIRE(a == b);
			REQUIRE(a.ToString10() == b.ToString10());
			REQUIRE(a.ToString16() == b.ToString16());

			REQUIRE(a.Parse(b.ToString10()) == Number_Parse_OK);
			REQUIRE(a == b);
			REQUIRE(a.Parse(b.ToString16()) == Number_Parse_OK);
			REQUIRE(a == b);
		}
	}

	SECTION("Parse wrong.") {
		std::string test_number_2[] = { "", "-", "0123", "0b1102",
			"0o123467758", "123743a", "0x23c4dfaeg", "0x-3df" };

		for (auto ele : test_number_2) {
			Integer a;
			REQUIRE(a.Parse(ele) == Number_Parse_Failed);
			REQUIRE(a == 0);
		}
	}
}

Integer RandomIntegerNumber() {
	static std::default_random_engine generator((unsigned)time(NULL));
	static std::uniform_int_distribution<save_type> distribution, size_d(1, 1024), sig(0, 1);
	static auto dice = std::bind(distribution, generator);
	int size = size_d(generator);
	std::vector<save_type> vec(size);
	for (auto& num : vec) {
		num = dice();
	}
	if (vec.back() == 0)
		++vec.back();
	return Integer(vec, sig(generator) ? 1 : -1);
}

TEST_CASE("Test Add and Sub for Integer", "[AddSub][Integer]") {
	SECTION("Special number test.") {
		std::tuple<std::string, std::string, std::string> test_number[] = { { "0", "0", "0" },
		{ "-0", "0", "0" },{ "0", "1", "1" },{ "0", "-3", "-3" },{ "-7", "7", "0" },
		{ "32768", "65536", "98304" },{ "10000", "1000000", "1010000" },{ "0x10000000", "0x10000", "0x10010000" } };

		for (auto ele : test_number) {
			Integer s, a, b;
			REQUIRE(a.Parse(std::get<0>(ele)) == Number_Parse_OK);
			REQUIRE(b.Parse(std::get<1>(ele)) == Number_Parse_OK);
			REQUIRE(s.Parse(std::get<2>(ele)) == Number_Parse_OK);
			REQUIRE(a + b == s);
			REQUIRE(b + a == s);
			REQUIRE(s - a == b);
			REQUIRE(s - b == a);
			REQUIRE(a - s == -b);
			REQUIRE(b - s == -a);
		}
	}

	SECTION("Random number test.") {
		for (int ii = 0;ii < integer_test_num;++ii) {
			Integer a = RandomIntegerNumber();
			Integer b = RandomIntegerNumber();
			Integer r1 = a + b;
			Integer r2 = r1 - a;
			REQUIRE(r2 == b);
		}
	}
}

TEST_CASE("Test Multiply and Devide for Integer", "[MulDev][Integer]") {
	SECTION("Special number test.") {
		std::tuple<std::string, std::string, std::string> test_number[] = { {"0", "0", "0"},
		{"-0", "0", "0"},
		{"0", "1", "0"},
		{"-3", "0", "0"},
		{"2", "70", "140"},
		{"-5", "30", "-150"},

		{"338745", "24325", "8239972125"},
		{"-60374", "22580", "-1363244920"},
		{"-34750247679359595104195192713491853248905703459234", "-1",
			"34750247679359595104195192713491853248905703459234"},
		{"34502938750234750245703459234857029503464564775029452934509287437676816581876214",
			"7490918801118394765103491091168307360192023441273509150198327409213539834673455",
			"258458712577969901107793595983076419519271833012883483684123496626476994193811141762278729300587831551290525355683871785611321045319590811232460657255921699370"},
		{"2384750193485324891786231019822357117632491019432579320923759071264016501924345",
			"-9965977871263490182375019843750912381720472136487651923245912386419827364981234",
			"-23766367656766074498187360121055082313211874311420286616610754823487880627777351412999960762652087319961498758734391202707977726944188275025244657025812741730"}
		};

		for (auto ele : test_number) {
			Integer s, a, b;
			REQUIRE(a.Parse(std::get<0>(ele)) == Number_Parse_OK);
			REQUIRE(b.Parse(std::get<1>(ele)) == Number_Parse_OK);
			REQUIRE(s.Parse(std::get<2>(ele)) == Number_Parse_OK);
			REQUIRE(a * b == s);
			REQUIRE(b * a == s);
			if (a != 0)
				REQUIRE(s / a == b);
			if (b != 0)
			REQUIRE(s / b == a);
		}
	}

	SECTION("Random number test.") {
		for (int ii = 0;ii < integer_test_num;++ii) {
			Integer a = RandomIntegerNumber() + 1;
			Integer b = RandomIntegerNumber();
			Integer r1 = a * b;
			Integer r2 = r1 / a;
			REQUIRE(r2 == b);
			Integer q = b / a;
			Integer m = b % a;
			r1 = q * a;
			r2 = r1 + m;
			REQUIRE(r2 == b);
		}
	}
}
