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

TEST_CASE("Test FFT_static", "[FFT][FFT_static][Algorithm]") {
    constexpr int length = 1024;
    double omega = 3.14159265358979 / 8;
    auto src = new std::complex<double>[length];
    auto fft = new std::complex<double>[length];

    for (int ii=0;ii<length;++ii) {
        src[ii] = cos(omega * ii);
        fft[ii] = cos(omega * ii);
    }

    FFT_static<length>(fft);
    IFFT_static<length>(fft);
    for (int ii=0;ii<length;++ii) {
        INFO("In " << ii << "-th number.");
        CAPTURE(src[ii]);
        CAPTURE(fft[ii]);
        REQUIRE(std::abs(src[ii]) == Approx(std::abs(fft[ii])));
    }
    
    delete [] src;
    delete [] fft;
}

// TEST_CASE("Test FFT_real_static", "[FFT][FFT_static][FFT_real][Algorithm]") {
//     constexpr int length = 1024;
//     double omega = 3.14159265358979 / 8;
//     auto src = new double[length];
//     auto fft = new double[length];

//     for (int ii=0;ii<length;++ii) {
//         src[ii] = cos(omega * ii);
//         fft[ii] = cos(omega * ii);
//     }

//     FFT_real_static<length>(src, fft);
//     IFFT_real_static<length>(fft, fft);
//     for (int ii=0;ii<length;++ii) {
//         INFO("In " << ii << "-th number.");
//         CAPTURE(src[ii]);
//         CAPTURE(fft[ii]);
//         REQUIRE(std::abs(src[ii]) == Approx(std::abs(fft[ii])));
//     }
    
//     delete [] src;
//     delete [] fft;
// }

TEST_CASE("Test FFT profile", "[FFT][Algorithm][profile]") {
    int length = 1024;
    double omega = 3.14159265358979 / 8;
	vector<double> src(length);
    vector<float> src_f(length);
    auto src_sd = new std::complex<double>[1024];
    auto src_sf = new std::complex<float>[1024];

    for (int ii=0;ii<length;++ii) {
        double val = cos(omega * ii);
        src[ii] = val;
        src_f[ii] = val;
        src_sd[ii] = std::complex<double>(val);
        src_sf[ii] = std::complex<float>(val);
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
    {
        RunningTime timer("1024 point double FFT_static");
        FFT_static<1024>(src_sd);
    }
    {
        RunningTime timer("1024 point float FFT_static");
        FFT_static<1024>(src_sf);
    }

    TestSpeed fft_d(
        [&](){auto res = FFT(src); return res[0].real();},
        1000, "1024 point double FFT");
    TestSpeed fft_f(
        [&](){auto res = FFT(src_f); return res[0].real();},
        1000, "1024 point double FFT");
    TestSpeed fft_sd(
        [&](){FFT_static<1024>(src_sd); return src_sf[0].real();},
        1000, "1024 point double FFT_static");
    TestSpeed fft_sf(
        [&](){FFT_static<1024>(src_sf); return src_sf[0].real();},
        1000, "1024 point double FFT_static");

    fft_d.test();
    fft_f.test();
    fft_sd.test();
    fft_sf.test();

    delete [] src_sd;
    delete [] src_sf;
}
