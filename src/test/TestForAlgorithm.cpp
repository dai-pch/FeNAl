#include "catch.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "number/algrm.h"
#include "tools/timer.h"

using namespace std;
using namespace Number;
using namespace FeTools;

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

TEST_CASE("Test FFT profile", "[FFT][Algorithm][profile]") {
    int length = 1024;
    double omega = 3.14159265358979 / 8;
	vector<double> src(length);
    vector<float> src_f(length);

    for (int ii=0;ii<length;++ii) {
        src[ii] = cos(omega * ii);
        src_f[ii] = cos(omega * ii);
    }

    {
        RunningTime timer("Blank baseline.");
    }
    //initialization
    {
        RunningTime timer("1024 point double FFT");
        auto fft = FFT(src);
    }
    {
        RunningTime timer("1024 point float FFT");
        auto fft = FFT(src_f);
    }

    TestSpeed fft_d(
        [&](){auto res = FFT(src); return res[0].real();},
        1000, "1024 point double FFT");
    fft_d.test();
    TestSpeed fft_f(
        [&](){auto res = FFT(src_f); return res[0].real();},
        1000, "1024 point double FFT");
    fft_f.test();
}
