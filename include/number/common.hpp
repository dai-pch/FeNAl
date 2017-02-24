//#include "Number.h"
namespace Number { namespace detail {
	inline void CalcTypeToSaveType(const calc_type_u CalcValue, save_type &High, save_type &Low) {
		Low = static_cast<save_type>(CalcValue & MASK_CODE);
		High = static_cast<save_type>(static_cast<calc_type_u>(CalcValue) >> BIT_NUMBER);
	}

	inline save_type FullAdder(const save_type& Num1, const save_type& Num2, save_type& c)
	{
		calc_type temp;
		temp = static_cast<calc_type>(Num1)
			+ static_cast<calc_type>(Num2)
			+ static_cast<calc_type>(c);
		save_type res;
		CalcTypeToSaveType(temp, c, res);
		return res;
	}

	//一位减法器，c=0代表借位，c=1代表不借位
	inline save_type FullSuber(const save_type& Num1, const save_type& Num2, save_type& c)
	{
		calc_type temp;
		temp = static_cast<calc_type>(Num1)
			- static_cast<calc_type>(Num2)
			+ static_cast<calc_type>(c)
			+ MODULE - 1;
		save_type res;
		CalcTypeToSaveType(temp, c, res);
		return res;
	}

	//除去可能的高位的零
	inline void eraseZero(std::vector<save_type> &vec) {
		auto it_re = vec.rbegin();
		size_t counter = 0;
		while ((it_re != vec.rend() - 1) && *it_re == 0)
		{
			++counter;
			++it_re;
		}
		vec.erase(vec.end() - counter, vec.end());
	}

	//逐位比较绝对值大小
	inline int _compare_by_digit(::std::vector<save_type>::const_reverse_iterator it1,
		::std::vector<save_type>::const_reverse_iterator it2,
		size_t n1, size_t n2)
	{
		size_t digit_num = std::max(n1, n2);
		for (size_t ii = 0;ii < digit_num;++ii) {
			auto a = ii < n1 ? *(it1++) : 0;
			auto b = ii < n2 ? *(it2++) : 0;
			if (a < b) {
				return -1;
			}
			else if (a > b) {
				return 1;
			}
		}
		return 0;
	}

	inline ::std::vector<save_type> multiply_vec(const ::std::vector<save_type>& number1,
		const ::std::vector<save_type>& number2) {
		auto size1 = number1.size(), size2 = number2.size();
		::std::vector<save_type> result(size1 + size2, 0);
		for (unsigned int ii = 0;ii < size1;ii++)
		{
			for (unsigned int jj = 0;jj < size2;jj++)
			{
				calc_type_u temp = static_cast<calc_type_u>(number1[ii]) * static_cast<calc_type_u>(number2[jj]);
				save_type high, low, c = 0;
				CalcTypeToSaveType(temp, high, low);
				result[ii + jj] = FullAdder(result[ii + jj], low, c);
				result[ii + jj + 1] = FullAdder(result[ii + jj + 1], high, c);
				if (c)
					++result[ii + jj + 2];
			}
		}
		if (result.back() == 0)
			result.pop_back();
		return result;
	}

	inline void convertFloatingToInteger(float number, signal_type& sig, ::std::vector<save_type>& res,
		exp_type& exp) {
		int32_t& src = reinterpret_cast<int32_t&>(number);
		calc_type_u temp = src & ((1 << 23) - 1);
		exp = (src >> 23) & ((1 << 8) - 1);
		sig = (src >> 31) ? -1 : 1;;
		if (exp == ((1 << 8) - 1))
			throw(::std::logic_error("Construct real number from NaN or Inf."));

		if (exp != 0)
			temp |= (1 << 23);
		else
			++exp;
		exp -= 127;
		exp -= 23;
		int exp_sh = (exp % (exp_type)BIT_NUMBER);
		if (exp_sh < 0)
			exp_sh += BIT_NUMBER;
		temp <<= exp_sh;
		exp = (exp - exp_sh) / (exp_type)BIT_NUMBER;
		res.clear();
		res.push_back(temp & MASK_CODE);
		if (temp >> BIT_NUMBER != 0) {
			res.push_back(temp >> BIT_NUMBER);
		}
	}

	inline void convertFloatingToInteger(double number, signal_type& sig, ::std::vector<save_type>& res,
		exp_type& exp) {
		int64_t& src = reinterpret_cast<int64_t&>(number);
		calc_type_u temp = src & (((int64_t)1 << 52) - 1);
		exp = (src >> 52) & ((1 << 11) - 1);
		sig = (src >> 63) ? -1 : 1;
		if (exp == ((1 << 11) - 1))
			throw(::std::logic_error("Construct real number from NaN or Inf."));

		if (exp != 0)
			temp |= ((int64_t)1 << 52);
		else
			++exp;
		exp -= 1023;
		exp -= 52;
		exp_type exp_sh = (exp % (exp_type)BIT_NUMBER);
		if (exp_sh < 0)
			exp_sh += BIT_NUMBER;
		save_type temp_h;
		temp_h = static_cast<save_type>(temp >> BIT_NUMBER);
		temp_h >>= (BIT_NUMBER - exp_sh);
		temp <<= exp_sh;
		exp = (exp - exp_sh) / (exp_type)BIT_NUMBER;
		res.clear();
		res.push_back(temp & MASK_CODE);
		if ((temp_h != 0 || (temp >> BIT_NUMBER != 0)))
			res.push_back(temp >> BIT_NUMBER);
		if (temp_h != 0)
			res.push_back(temp_h);
	}

	template<typename T>
	inline void NumberParseSignal(T& SrcIt, char& Signal) {
		Signal = 1;
		if (*SrcIt == '-')
		{
			++SrcIt;
			Signal = -1;
		}
		else if (*SrcIt == '+')
			++SrcIt;
	}

}; // detail

template<typename IntType>
inline bool IsEven(IntType num, ::std::void_t<decltype(num & 0)>* = nullptr) {
	return !(bool)(num & 1);
}

}; // Number
