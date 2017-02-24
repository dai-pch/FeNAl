#include "catch.hpp"

#include <functional>
#include <cmath>
#include "number/algrm.h"
using namespace Number;
using namespace std;

TEST_CASE("Test simpson", "[Simpson][Integration]") {
	SECTION("square function") {
		auto f = [](double x) {return x * x;};
		auto g = [](double x) {return std::sin(x);};
		double sim = simpson(f, 0.0, 1.0);
		double sim_cos = simpson(g, 0.0, 3.141592653589793238462);
		double ads = adaptiveSimpson(f, 0.0, 1.0, 0.000001);
		double ads_cos = adaptiveSimpson(g, 0.0, 3.141592653589793238462, 0.0000000001);
		REQUIRE(sim == Approx(0.333333333333333333333));
		REQUIRE(ads == Approx(0.333333333333333333333).epsilon(0.000001));
		REQUIRE(ads_cos == Approx(2.0).epsilon(0.0000000001));
	}
}
