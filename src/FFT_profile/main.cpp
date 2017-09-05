#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "number/algrm.h"
#include "tools/timer.h"

using namespace std;
using namespace Number;
using namespace FeTools;

constexpr int length = 1024;
double omega = 3.14159265358979 / 8;
vector<double> src(length);
std::complex<double> src_s[length];

void prepare() {
    for (int ii=0;ii<length;++ii) {
        src[ii] = cos(omega * ii);
        src_s[ii] = cos(omega * ii);
    }
    auto fft = FFT(src);
    FFT_static<length>(src_s);
}

void test_fft(int times) {
    RunningTime timer("1024 point double FFT");
    for (int ii=0;ii<times;++ii) {
        auto fft = FFT(src);
    }
}

void test_fft_static(int times) {
    RunningTime timer("1024 point double static FFT");
    for (int ii=0;ii<times;++ii) {
        FFT_static<length>(src_s);
    }
}

int main(int argc, char * argv[]) {
    int times = 1000;
    if (argc > 1)
        times = atoi(argv[1]);
    prepare();
    test_fft(times);
    test_fft_static(times);
    return 0;
}
