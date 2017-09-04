#pragma once
#ifndef _NUMBER_ALGRM_H
#define _NUMBER_ALGRM_H

#include "Number.h"
#include <functional>
#include <vector>
#include <complex>

namespace Number {

	template<typename T>
	T Devide(const T& Obj1, const T& Obj2, T &mod, typename std::enable_if_t<
		std::is_integral<T>::value>* = nullptr)
	{
		mod = Obj1 % Obj2;
		return Obj1 / Obj2;
	}

	template<typename T>
	T ExGCD(const T &a, const T &b, T &x, T &y,
		typename std::enable_if_t<
		(std::is_integral<T>::value) || (std::is_same<T, Integer>::value)
		>* = nullptr)
	{
		if (a < b)
			return ExGCD(b, a, y, x);
		else if (b == 0)
		{
			x = 1;
			y = 0;
			return a;
		}
		else
		{
			T r, q;
			Devide(a, b, r, q);
			r = ExGCD(b, r, y, x);
			y = y - q * x;
			return r;
		}
	}

	//adaptiveSimpson
	template<typename DataType>
	inline DataType _simpson(DataType h, DataType f_a, DataType f_c, DataType f_b) {
		return (f_a + (DataType)4 * f_c + f_b) * h / (DataType)6;
	}

	template<typename DataType, typename FuncType = DataType(DataType)>
	inline DataType simpson(FuncType F, DataType a, DataType b) {
		DataType f_a = F(a);
		DataType f_b = F(b);
		DataType h = b - a;
		DataType c = (a + b) / 2;
		DataType f_c = F(c);
		return _simpson(h, f_a, f_c, f_b);
	}

	template<unsigned MaxTime, typename FuncType, typename DataType>
	inline DataType _adaptiveSimpson(FuncType F, DataType a, DataType c, DataType b,
		DataType f_a, DataType f_c, DataType f_b, DataType S,
		DataType epsilon, unsigned IterTimes) {
		if (IterTimes > MaxTime) {
			throw std::runtime_error("Iterate times too much in adaptive simpson method.");
		}
		DataType d = (a + c) / 2;
		DataType e = (b + c) / 2;
		DataType f_d = F(d);
		DataType f_e = F(e);
		DataType S_L = _simpson(c - a, f_a, f_d, f_c);
		DataType S_R = _simpson(b - c, f_c, f_e, f_b);
		DataType err = (S_L + S_R - S) / 15;
		err = err < 0 ? -err : err;
		if (3 * err < epsilon)
			return S_L + S_R + err;
		else
		{
			S_L = _adaptiveSimpson<MaxTime>(F, a, d, c, f_a, f_d, f_c, S_L, epsilon / 2, IterTimes + 1);
			S_R = _adaptiveSimpson<MaxTime>(F, c, e, b, f_c, f_e, f_b, S_R, epsilon / 2, IterTimes + 1);
			return S_L + S_R;
		}
	}

	template<typename DataType, unsigned MaxTime = 100, typename FuncType = DataType(DataType)>
	inline DataType adaptiveSimpson(FuncType F, DataType a, DataType b, DataType err) {
		assert(err > 0);
		DataType c = (a + b) / 2;
		DataType f_a = F(a);
		DataType f_b = F(b);
		DataType f_c = F(c);
		DataType S = _simpson(b - a, f_a, f_c, f_b);
		return _adaptiveSimpson<MaxTime>(F, a, c, b, f_a, f_c, f_b, S, err, 0);
	}

	inline size_t _fft_reverse(size_t a, unsigned n) {
		if (n == 1)
			return a;
		unsigned n_l = n/2;
		unsigned n_r = n - n_l;
		size_t l = (a >> n_r) & ((1 << n_l) - 1);
		size_t r = a & ((1 << n_r) - 1);
		return (_fft_reverse(r, n_r) << n_l) | _fft_reverse(l, n_l);
	}

	inline void _fft_size(size_t src_len, size_t& length, unsigned& n) {
		assert(src_len >= 2);
		// calc length
		length = 2;
		n = 1;
		while (length < src_len) {
			length *= 2;
			n += 1;
		}
	}

	template <typename T>
	inline void _radix2_fft(std::vector<std::complex<T>>& res, size_t length) {
		const double pi = 3.141592653589793238462;
		// calc W
		static std::vector<std::complex<T>> W;
		if (W.size() * 2 < length){
			W.clear();
			W.reserve(length/2);
			for (size_t ii=0;ii<length/2;++ii){
				T theta = pi * ii / (length / 2);
				W.push_back(std::complex<T>(cos(theta), -sin(theta)));
			}
		}

		// fft inplace
		for (size_t p = 2;p <= length;p *= 2) {
			for (size_t q =0;q<length;q+=p) {
				for (size_t ii=0;ii<p/2;++ii) {
					auto W_ = W[ii * (W.size() * 2 / p)]; // W_p^ii
					auto temp = res[q + ii];
					auto temp2 = res[q + p/2 + ii] * W_;
					res[q + ii] = temp + temp2;
					res[q + p/2 + ii] = temp - temp2;
				}
			}
		}
	}

	template <typename T, unsigned Len>
	struct _calc_radix2_fft{
		inline static void calc(std::complex<T>* res, const std::vector<std::complex<T>>& W) {
			constexpr unsigned Lend2 = Len / 2;
			_calc_radix2_fft<T, Lend2>::calc(res, W);
			_calc_radix2_fft<T, Lend2>::calc(res + Lend2, W);
			for (size_t ii = 0;ii < Lend2;++ii) {
				auto W_ = W[ii * (W.size() * 2 / Len)]; // W_p^ii
				auto temp = res[ii];
				auto temp2 = res[Lend2 + ii] * W_;
				res[ii] = temp + temp2;
				res[Lend2 + ii] = temp - temp2;
			}
		}
	};

	template <typename T>
	struct _calc_radix2_fft<T, 2> {
		inline static void calc(std::complex<T>* res, const std::vector<std::complex<T>>& W) {
			auto temp = res[0];
			auto temp2 = res[1];
			res[0] = temp + temp2;
			res[1] = temp - temp2;
		}
	};

	template <typename T>
	struct _calc_radix2_fft<T, 4> {
		inline static void calc(std::complex<T>* res, const std::vector<std::complex<T>>& W) {
			std::complex<T> i(0, 1);
			auto x0 = res[0];
			auto x1 = res[1];
			auto x2 = res[2];
			auto x3 = res[3];
			auto x2i = std::complex<T>(-x2.imag(), x2.real());
			auto x3i = std::complex<T>(-x3.imag(), x3.real());
			res[0] = x0 + x1 + x2 + x3;
			res[1] = x0 - x1 - x2i + x3i;
			res[2] = x0 + x1 - x2 - x3;
			res[3] = x0 - x1 + x2i - x3i;
		}
	};

	template <typename T, unsigned Len>
	inline void _radix2_fft_static(std::complex<T>* res) {
		const double pi = 3.141592653589793238462;
		// calc W
		static std::vector<std::complex<T>> W;
		if (W.size() < Len / 2){
			W.clear();
			W.reserve(Len/2);
			for (size_t ii=0;ii<Len/2;++ii){
				T theta = pi * ii / (Len / 2);
				W.push_back(std::complex<T>(cos(theta), -sin(theta)));
			}
		}

		// fft inplace
		_calc_radix2_fft<T, Len>::calc(res, W);
	}

	template <size_t Len, typename T>
	void FFT_static(std::complex<T>* res) {
		unsigned n;
		size_t length;
		_fft_size(Len, length, n);
		assert(length == Len);
		// resort
		for (unsigned ii=0;ii<Len;++ii) {
			auto des = _fft_reverse(ii, n);
			if (des > ii)
				std::swap(res[des], res[ii]);
		}

		_radix2_fft_static<T, Len>(res);
		return;
	}
	template <size_t Len, typename T>
	void IFFT_static(std::complex<T>* res) {
		unsigned n;
		size_t length;
		_fft_size(Len, length, n);
		assert(length == Len);
		//conj and scalar in
		for (unsigned ii=0;ii<Len;++ii) {
			res[ii] = std::conj(res[ii]) / static_cast<T>(Len);
		}
		// resort
		for (unsigned ii=0;ii<Len;++ii) {
			auto des = _fft_reverse(ii, n);
			if (des > ii)
				std::swap(res[des], res[ii]);
		}
		
		_radix2_fft_static<T, Len>(res);
	}

	// template <size_t Len, typename T>
	// std::vector<std::complex<T>> FFT_real_static(const std::vector<T>& src) {
	// 	unsigned n;
	// 	size_t length;
	// 	_fft_size(Len, length, n);
	// 	assert(length == Len);
	// 	// resort
	// 	std::complex<T>* res = new std::complex<T>[Len];
	// 	for (unsigned ii=0;ii<Len;++ii) {
	// 		auto des = _fft_reverse(ii, n);
	// 		res[des] = std::complex<T>(src[ii], 0);
	// 	}

	// 	_radix2_fft_static<T, Len>(res);
	// 	std::vector<std::complex<T>> ret(Len);
	// 	for (int ii = 0;ii < Len;++ii)
	// 		ret[ii] = res[ii];

	// 	delete [] res;
	// 	return ret;
	// }

	template <typename T>
	std::vector<std::complex<T>> FFT(const std::vector<T>& src) {
		size_t length;
		unsigned n;
		_fft_size(src.size(), length, n);
		// resort
		std::vector<std::complex<T>> res(length);
		for (unsigned ii=0;ii<length;++ii) {
			auto des = _fft_reverse(ii, n);
			res[des] = std::complex<T>(src[ii], 0);
		}

		_radix2_fft(res, length);
		return res;
	}
	template <typename T>
	std::vector<T> IFFT(const std::vector<std::complex<T>>& src) {
		size_t length;
		unsigned n;
		_fft_size(src.size(), length, n);
		// resort
		std::vector<std::complex<T>> conject(length);
		for (unsigned ii=0;ii<length;++ii)
			conject[_fft_reverse(ii, n)] = std::conj(src[ii]);
		
		_radix2_fft(conject, length);
		std::vector<T> res(length);
		for (unsigned ii=0;ii<length;++ii)
			res[ii] = conject[ii].real() / length;
		return res;
	}

} // namespace Number

#endif // _NUMBER_ALGRM_H
