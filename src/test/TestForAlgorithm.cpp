#include "catch.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "number/algrm.h"

using namespace std;
using namespace Number;

TEST_CASE("Test FFT", "[FFT][Algorithm]") {
    int length = 1024;
    double omega = 3.14159265358979 / 8;
	vector<double> src(length);

    for (int ii=0;ii<length;++ii) {
        src[ii] = cos(omega * ii);
    }

    auto fft = FFT(src);
    auto ifft = IFFT(fft);
    CAPTURE(src);
    CAPTURE(fft);
    CAPTURE(ifft);
    for (int ii=0;ii<length;++ii) {
        INFO("In " << ii << "-th number.");
        REQUIRE(src[ii] == Approx(ifft[ii]));
    }

}
