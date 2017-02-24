#pragma once
#ifndef _NUMBER_ALGRM_H
#define _NUMBER_ALGRM_H

#include "Number.h"
#include <functional>

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
}

#endif
