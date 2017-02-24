#pragma once
#ifndef _NUMBER_INTEGER_H__
#define _NUMBER_INTEGER_H__

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include "Number.h"
#include "UInteger.h"

namespace Number {
	class Integer {
		friend class Real;
	public:
		Integer() = default;
		Integer(const Integer&);

		Integer(const std::vector<save_type>& Number, char Signal = 1);
		Integer(std::vector<save_type>&& Number, char Signal = 1);
		//类型转换
		explicit Integer(const UInteger&, const char Signal = 1);

		template<typename T>
		explicit Integer(const T& Source);

		// 赋值操作
		Integer& operator=(const Integer&);
		Integer& operator=(Integer&&);
		//无符号整数
		template<typename UIntType>
		typename std::enable_if_t<std::is_integral<UIntType>::value 
			&& std::is_unsigned<UIntType>::value, Integer>&
			operator = (const UIntType& Source);
		//有符号整数
		template<typename IntType>
		typename std::enable_if_t<std::is_integral<IntType>::value 
			&& std::is_signed<IntType>::value, Integer>&
			operator=(const IntType& Source);
		//比较运算
		int Compare(const Integer&) const; //小于输出负数，大于输出正数，等于输出零

		//逻辑操作符
		Integer operator<<(int) const;
		Integer operator>>(int) const;


		// 算数操作
		// Minus
		Integer operator-() const;
		// Add
		Integer Add(const Integer& Number2) const;
		// Sub
		Integer Sub(const Integer& Number2) const;
		// Multiply
		Integer Multiply(const Integer& Number2) const;
		// Devide
		friend void Devide(const Integer &Number1, const Integer &Number2,
			Integer &Mod, Integer& quotient);

		//转换为10进制字串
		std::string ToString10() const;
		std::string ToString16() const;

		int Parse(std::string);

		size_t size() const;
		//save_type get_highest_digit() const;

		//输入输出
		friend std::ostream& operator<<(std::ostream&, const Integer&);
		friend std::istream& operator>>(std::istream&, Integer&);

		//其他算数运算
		UInteger Abs() const;
		Integer Power(save_type exp) const;
		Integer Power(const UInteger &exp) const;

	private:
		char _signal = 1;
		UInteger _number{ (unsigned)0 };
		std::vector<save_type>& _numvec = _number._number;

		int setSignal(const int& signal);

	}; // class

	// compare operator
	// ==运算符
	inline bool operator == (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) == 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator == (const IntType& Number1, const Integer& Number2) {
		return (Number2 == Integer(Number1));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator == (const Integer& Number1, const IntType& Number2) {
		return (Number1 == Integer(Number2));
	}
	// !=运算符
	inline bool operator != (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) != 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator != (const IntType& Number1, const Integer& Number2) {
		return (Number2 != Integer(Number1));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator != (const Integer& Number1, const IntType& Number2) {
		return (Number1 != Integer(Number2));
	}
	// >运算符
	inline bool operator > (const Integer &number1, const Integer &number2) {
		return ((number1.Compare(number2)) > 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator > (const Integer& Number1, const IntType& Number2)	{
		return (Number1 > Integer(Number2));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator > (const IntType& Number1, const Integer& Number2)	{
		return (Integer(Number1) > Number2);
	}
	// <运算符	
	inline bool operator < (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) < 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator < (const IntType& Number1, const Integer& Number2)	{
		return (Integer(Number1) < Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator < (const Integer& Number1, const IntType& Number2)	{
		return (Number1 < Integer(Number2));
	}
	// >=运算符
	inline bool operator >= (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) >= 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator >= (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) >= Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator >= (const Integer& Number1, const IntType& Number2) {
		return (Number1 >= Integer(Number2));
	}
	// <=运算符
	inline bool operator <= (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) <= 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator <= (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) <= Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator <= (const Integer& Number1, const IntType& Number2) {
		return (Number1 <= Integer(Number2));
	}
	// operator +
	inline Integer operator + (const Integer& Number1, const Integer& Number2) {
		return Number1.Add(Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator + (const IntType& Number1, const Integer& Number2) {
		return (Number2 + Integer(Number1));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator + (const Integer& Number1, const IntType& Number2) {
		return (Number1 + Integer(Number2));
	}
	// operator -
	inline Integer operator - (const Integer& Number1, const Integer& Number2) {
		return Number1.Sub(Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator - (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) - Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator - (const Integer& Number1, const IntType& Number2) {
		return (Number1 - Integer(Number2));
	}
	// operator *
	inline Integer operator * (const Integer& Number1, const Integer& Number2) {
		return Number1.Multiply(Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator * (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) * Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator * (const Integer& Number1, const IntType& Number2) {
		return (Number1 * Integer(Number2));
	}
	// operator /
	inline Integer operator / (const Integer& Number1, const Integer& Number2) {
		Integer q, r;
		Devide(Number1, Number2, r, q);
		return q;
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator / (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) / Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator / (const Integer& Number1, const IntType& Number2) {
		return (Number1 / Integer(Number2));
	}
	// operator %
	inline Integer operator % (const Integer& Number1, const Integer& Number2) {
		Integer q, r;
		Devide(Number1, Number2, r, q);
		return r;
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator % (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) % Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator % (const Integer& Number1, const IntType& Number2) {
		return (Number1 % Integer(Number2));
	}

	template<typename T>
	Integer::Integer(const T& Source) {
		*this = Source;
	}

	template<typename UIntType>
	typename std::enable_if_t<std::is_integral<UIntType>::value
		&& std::is_unsigned<UIntType>::value, Integer>&
		Integer::operator=(const UIntType& Source)
	{
		_number = Source;
		return *this;
	}

	template<typename IntType>
	typename std::enable_if_t<
		std::is_integral<IntType>::value && std::is_signed<IntType>::value, Integer>&
		Integer::operator=(const IntType& Source)
	{
		std::make_unsigned_t<IntType> temp = (Source < 0 ? (_signal = -1, -Source) : Source);
		_number = temp;
		return *this;
	}

}

//#include "Integer_implement.hpp"

#endif