#include "catch.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "number/Matrix.h"
#include "signal_processing/Kalman.h"

using namespace std;
using namespace Number;
using namespace SigPro;

TEST_CASE("Test Kalman Filter", "[Kalman]") {
	int max_time = 100;
	double velocity = 5;
	double init_x = 10;
	vector<double> real_x(max_time);
	for (int i=0;i<max_time;++i)
		real_x[i] = init_x + velocity * i;

	double q_v = 1;
	double r_v = 4;
	auto r = Matrix<double>(1, 1, vector<double>{r_v});
	auto q = Matrix<double>(1, 1, vector<double>{q_v});
	auto s_var = Matrix<double>(1, 1, vector<double>{1});

	auto s_init = Matrix<double>(1, 1);
	auto A = Matrix<double>(1, 1, vector<double>{1});
	auto B = Matrix<double>(1, 1, vector<double>{1});
	auto H = Matrix<double>(1, 1, vector<double>{1});

	KalmanFilter a(s_init, s_var, A, q, H, r, B);

	vector<double> calc_x(max_time);
	random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> normal(0,1);
	calc_x[0] = 0;
	for (int t=1;t<max_time;++t) {

		double u_v = velocity + q_v * normal(gen);
		double y_v = real_x[t] + r_v * normal(gen);
		auto u = Matrix<double>(1, 1, vector<double>{u_v});
		auto y = Matrix<double>(1, 1, vector<double>{y_v});

		a.step(u, y);
		calc_x[t] = a.state()(0, 0);
		INFO(calc_x[t] << "; " << real_x[t] << "; " << a.covariance()(0, 0));
	}

	REQUIRE(std::abs(calc_x[max_time - 1] - real_x[max_time - 1]) < 3 * a.covariance()(0, 0));
	
}

TEST_CASE("Test EKF", "[EKF]") {
	int max_time = 100;
	double velocity = 5;
	double init_x = 10;
	vector<double> real_x(max_time);
	for (int i=0;i<max_time;++i)
		real_x[i] = init_x + velocity * i;

	double q_v = 1;
	double r_v = 4;
	auto r = Matrix<double>(1, 1, vector<double>{r_v});
	auto q = Matrix<double>(1, 1, vector<double>{q_v});
	auto s_var = Matrix<double>(1, 1, vector<double>{1});

	auto s_init = Matrix<double>(1, 1);
	auto A = [](Matrix<double> state, Matrix<double> con){
		return state + con;
	};
	auto A_prime = [](Matrix<double> state, Matrix<double> con){
		return Matrix<double>(1, 1, vector<double>{1});
	};
	auto H = [](Matrix<double> obs){
		return obs;
	};
	auto H_prime = [](Matrix<double> obs){
		return Matrix<double>(1, 1, vector<double>{1});
	};

	EKF a(s_init, s_var, A, A_prime, q, H, H_prime, r);

	vector<double> calc_x(max_time);
	random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> normal(0,1);
	calc_x[0] = 0;
	for (int t=1;t<max_time;++t) {

		double u_v = velocity + q_v * normal(gen);
		double y_v = real_x[t] + r_v * normal(gen);
		auto u = Matrix<double>(1, 1, vector<double>{u_v});
		auto y = Matrix<double>(1, 1, vector<double>{y_v});

		a.step(u, y);
		calc_x[t] = a.state()(0, 0);
		INFO(calc_x[t] << "; " << real_x[t] << "; " << a.covariance()(0, 0));
	}

	REQUIRE(std::abs(calc_x[max_time - 1] - real_x[max_time - 1]) < 3 * a.covariance()(0, 0));
	
}
