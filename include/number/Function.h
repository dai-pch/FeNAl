#pragma once
#ifndef _NUMBER_FUNCTION_H
#define _NUMBER_FUNCTION_H

#include <vector>
#include <cassert>
#include <algorithm>
#include <type_traits>

namespace Number{
	template<typename ReturnType, typename ... ValueType>
	class Function {
	public:
		virtual ~Function() {}

		virtual ReturnType operator()(const ValueType& ...) const = 0;
	};

	template<typename CoeffcientType, typename ValueType>
	class Polynomial : Function<ValueType, ValueType> {
	public:
		Polynomial() = default;
		Polynomial(const Polynomial&) = default;
		Polynomial(const std::vector<CoeffcientType>& coeffcients) : _coeffcients(coeffcients) {
			assert(coeffcients.size() > 0);
		}
		Polynomial(const CoeffcientType& coeffcient) : _coeffcients{ coeffcient } {}

		void SetCoeffcients(const std::vector<CoeffcientType>& coeffcients) {
			assert(coeffcients.size() > 0);
			_coeffcients = coeffcients;
		}

		Polynomial& operator=(const Polynomial& x) {
			_coeffcients = x._coeffcients;
			return *this;
		}

		Polynomial& operator=(Polynomial&& x) {
			_coeffcients = x._coeffcients;
			return *this;
		}

		// Qin Jiushao algorithm
		ValueType operator()(const ValueType& x) const {
			return _qinJiushao<ValueType>(x);
		}

		// Reduction
		Polynomial operator()(const Polynomial& y) const {
			return _qinJiushao<Polynomial>(y);
		}

		Polynomial operator-() const {
			auto sz = _coeffcients.size();
			std::vector<CoeffcientType> coe(sz, 0);
			for (auto ii = 0;ii < sz;++ii) {
				coe[ii] = -_coeffcients[ii];
			}
			return Polynomial(coe);
		}

		Polynomial operator+(const Polynomial& y) const {
			size_t x_sz = _coeffcients.size();
			size_t y_sz = y._coeffcients.size();
			size_t z_sz = std::max(x_sz, y_sz);
			std::vector<CoeffcientType> coe(z_sz, 0);
			for (size_t ii = 0;ii < z_sz;++ii) {
				CoeffcientType co_x = ii < x_sz ? _coeffcients[ii] : 0;
				CoeffcientType co_y = ii < y_sz ? y._coeffcients[ii] : 0;
				coe[ii] = co_x + co_y;
			}
			Polynomial res(coe);
			res._eraseZero();
			return res;
		}

		Polynomial operator-(const Polynomial& y) const {
			size_t x_sz = _coeffcients.size();
			size_t y_sz = y._coeffcients.size();
			size_t z_sz = std::max(x_sz, y_sz);
			std::vector<CoeffcientType> coe(z_sz, 0);
			for (size_t ii = 0;ii < z_sz;++ii) {
				CoeffcientType co_x = ii < x_sz ? _coeffcients[ii] : 0;
				CoeffcientType co_y = ii < y_sz ? y._coeffcients[ii] : 0;
				coe[ii] = co_x - co_y;
			}
			Polynomial res(coe);
			res._eraseZero();
			return res;
		}

		Polynomial operator*(const Polynomial& y) const {
			size_t x_order = _coeffcients.size() - 1;
			size_t y_order = y._coeffcients.size() - 1;
			size_t f_order = x_order + y_order;
			std::vector<CoeffcientType> coe(f_order + 1, 0);
			for (size_t ii = 0;ii <= x_order;++ii) {
				for (size_t jj = 0;jj <= y_order;++jj) {
					CoeffcientType co_x = _coeffcients[ii];
					CoeffcientType co_y = y._coeffcients[jj];
					coe[ii + jj] = coe[ii + jj] + co_x * co_y;
				}
			}
			return Polynomial(coe);
		}

		Polynomial derivative() const {
			size_t order = _coeffcients.size() - 1;
			if (order == 0)
				return Polynomial(0);
			std::vector<CoeffcientType> coe(order, 0);
			for (size_t ii = 1;ii <= order;++ii) {
				coe[ii - 1] = _coeffcients[ii] * (ii);
			}
			return Polynomial(coe);
		}

		std::vector<ValueType> Root(ValueType epsilon = 0.0000000001) const {
			std::vector<ValueType> res;
			assert(_coeffcients.size() > 0);
			if (_coeffcients.size() == 1)
				return res;

			CoeffcientType e = _findUpperBoundOfRoot();
			std::vector<CoeffcientType>tempvec = { e, -1 };
			Polynomial tempPoly = this->operator()(Polynomial(tempvec));
			// find the upperBound of all roots
			CoeffcientType upperBound = tempPoly._findUpperBoundOfRoot();
			if (upperBound <= 0)
				return res;
			res = tempPoly._root(upperBound, epsilon);
			for (auto& item : res) {
				item = e - item;
			}
			return res;
		}

	private:
		std::vector<CoeffcientType> _coeffcients{ 0 };

	private:
		template<typename vType>
		vType _qinJiushao(const vType& x) const {
			vType res = _coeffcients.back();
			for (auto it = _coeffcients.rbegin() + 1; it != _coeffcients.rend();++it) {
				res = res * x + *it;
			}
			return res;
		}

		void _eraseZero() {
			auto it = std::find_if_not(_coeffcients.rbegin(), _coeffcients.rend(),
				[](CoeffcientType c) {return c == 0;});
			if (it == _coeffcients.crend())
				--it;
			_coeffcients.erase(it.base(), _coeffcients.end());
		}

		//assume all roots are bigger then 0
		std::vector<ValueType> _root(ValueType upperBound, ValueType epsilon = 0.0000000001) const {
			std::vector<ValueType> res;
			assert(_coeffcients.size() > 1);
			assert(upperBound > 0);
			// polynomial of one degree
			if (_coeffcients.size() == 2){
				ValueType root = -(ValueType)_coeffcients[0] / (ValueType)_coeffcients[1];
				res.push_back(root);
				return res;
			}

			std::vector<ValueType> temproot = this->derivative()._root(upperBound, epsilon);
			// boundary
			temproot.insert(temproot.cbegin(), 0);
			temproot.push_back(upperBound);
			// find all roots
			for (size_t ii = 0;ii < temproot.size() - 1;++ii) {
				ValueType root;
				if (_dichotomie(temproot[ii], temproot[ii + 1], root, epsilon))
					res.push_back(root);
			}
			auto rm = std::unique(res.begin(), res.end());
			res.erase(rm, res.end());
			return res;
		}

		// assume a <= b.
		bool _dichotomie(ValueType a, ValueType b, ValueType& res, ValueType epsilon = 0.0000000001) const {
			assert(a <= b);
			auto f_a = operator()(a);
			auto f_b = operator()(b);
			if (f_a == 0) {
				res = a;
				return true;
			}
			else if (f_b == 0) {
				res = b;
				return true;
			}
			else if ((f_a > 0) == (f_b > 0))
				return false;

			bool flag = f_a > 0;
			assert(flag == (f_b < 0));
			while (b - a > epsilon) {
				ValueType c = (a + b) / 2;
				auto f_c = operator()(c);
				if (f_c == 0) {
					res = c;
					break;
				}
				else if (flag == f_c > 0) // f_a f_c have same sign.
					a = c;
				else
					b = c;
			}
			res = (a + b) / 2;
			return true;
		}

		CoeffcientType _findUpperBoundOfRoot() const {
			CoeffcientType e;
			if (_coeffcients.back() > 0)
				e = -*std::min_element(_coeffcients.begin(), _coeffcients.end());
			else
				e = -*std::max_element(_coeffcients.begin(), _coeffcients.end());
			e /= _coeffcients.back();
			e += (CoeffcientType)2;
			if (e < 0)
				e = 0;
			return e;
		}

	};
} // namespace Number

#endif
