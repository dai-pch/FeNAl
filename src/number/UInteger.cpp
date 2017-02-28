#include "UInteger.h"
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <cmath>

using std::vector;
namespace Number {
	UInteger::UInteger(const UInteger& src) : _number(src._number) {}
	UInteger::UInteger(const vector<save_type>& src) : _number(src) {}
	UInteger::UInteger(vector<save_type>&& src)
		: _number(std::forward<vector<save_type>>(src)) {}

	UInteger::operator save_type() const
	{
		return _number[0];
	}

	UInteger & UInteger::operator=(const UInteger &src)
	{
		_number = src._number;
		return *this;
	}

	UInteger & UInteger::operator=(UInteger &&src)
	{
		_number.swap(src._number);
		return *this;
	}

	int UInteger::Compare(const UInteger &Obj2) const
	{
		size_t size1 = _number.size(), size2 = Obj2._number.size();
		//位数不同直接判断
		if (size1 != size2)
			return (size1 - size2);
		return detail::_compare_by_digit(this->_number.crbegin(), Obj2._number.crbegin(), size1, size2);
	}

	vector<save_type> shift_left(const vector<save_type>& src, int num) {
		assert(num >= 0);
		assert(!src.empty());
		if (src.back() == 0 && src.size() == 1)
			return vector<save_type>(src);
		const unsigned int num_s = num % (BIT_NUMBER);
		const unsigned int num_sr = BIT_NUMBER - num_s;
		const unsigned int num_i = num / (BIT_NUMBER);
		auto size = src.size();
		vector<save_type> result(size + num_i);
		
		if (num_s == 0)
			std::copy(src.crbegin(), src.crend(), result.rbegin());
		else {
			result[num_i] = src[0] << num_s;
			for (decltype(size) ii = 1; ii < size; ii++)
			{
				result[ii + num_i] = (src[ii] << num_s) | (src[ii - 1] >> num_sr);
			}
			if ((src.back() >> num_sr) != 0)
				result.push_back(src[size - 1] >> num_sr);
		}
		return result;
	}

	vector<save_type> shift_right(const vector<save_type>& src, int num) {
		assert(num >= 0);
		assert(!src.empty());
		const unsigned int num_s = num % (BIT_NUMBER);
		const unsigned int num_sr = BIT_NUMBER - num_s;
		const unsigned int num_d = num / (BIT_NUMBER);
		auto size = src.size();

		if (num_d >= size)
			return vector<save_type>{0};
		vector<save_type> result(size - num_d);
		if (num_s == 0)
			std::copy(src.crbegin(), src.crend() - num_d, result.rbegin());
		else {
			for (unsigned int ii = 0;ii < size - num_d - 1;ii++)
			{
				result[ii] = (src[ii + num_d] >> num_s) | (src[ii + num_d + 1] << num_sr);
			}
			result[size - num_d - 1] = src[size - 1] >> num_s;

			if (result.back() == 0 && size != 1)
				result.pop_back();
		}
		return result;
	}

	UInteger UInteger::operator<<(int num) const
	{
		vector<save_type> result;
		if (num < 0)
			result = shift_right(_number, -num);
		else
			result = shift_left(_number, num);
		return UInteger(result);
	}

	UInteger UInteger::operator>>(int num) const
	{
		vector<save_type> result;
		if (num < 0)
			result = shift_left(_number, -num);
		else
			result = shift_right(_number, num);
		return UInteger(result);
	}

	UInteger UInteger::Add(const UInteger& Obj2) const
	{
		const UInteger *p1, *p2;
		this->_number.size() > Obj2._number.size() ? (p1 = &Obj2, p2 = this) : (p1 = this, p2 = &Obj2);

		unsigned int size1 = p1->_number.size(), size2 = p2->_number.size();
		vector<save_type> result(size2);

		calc_type temp = 0;
		save_type c = 0;
		unsigned int ii = 0;
		for (;ii < size1;ii++)
		{
			result[ii] = detail::FullAdder(p1->_number[ii], p2->_number[ii], c);
		}

		for (;ii < size2;ii++)
		{
			result[ii] = detail::FullAdder(0, p2->_number[ii], c);
		}

		if (c != 0)
			result.push_back(c);

		return UInteger(result);
	}

	UInteger UInteger::Sub(const UInteger& Obj2) const {
		assert(*this >= Obj2);
		const UInteger* p1 = this;
		const UInteger* p2 = &Obj2;

		size_t size1 = p1->_number.size(), size2 = p2->_number.size();
		vector<save_type> result;
		calc_type temp = 0;
		save_type c = 1; //参见一位减法器说明
		size_t ii = 0;
		auto re_it = std::back_inserter(result);
		for (;ii < size2;ii++)
		{
			*re_it = detail::FullSuber(p1->_number[ii], p2->_number[ii], c);
		}

		for (;ii < size1;ii++)
		{
			*re_it = detail::FullSuber(p1->_number[ii], 0, c);
		}

		//除去可能的高位的零
		detail::eraseZero(result);

		return UInteger(result);
	}

	UInteger UInteger::Multiply(const UInteger& Obj2) const {
		auto& number1 = this->_number;
		auto& number2 = Obj2._number;
		if (number1.back() == 0 || number2.back() == 0)
			return UInteger((unsigned)0);
		unsigned int size1 = number1.size(), size2 = number2.size();
		vector<save_type> result(size1 + size2, 0);
		for (unsigned int ii = 0;ii < size1;ii++)
		{
			for (unsigned int jj = 0;jj < size2;jj++)
			{
				calc_type_u temp = static_cast<calc_type_u>(number1[ii]) * static_cast<calc_type_u>(number2[jj]);
				save_type high, low, c = 0;
				detail::CalcTypeToSaveType(temp, high, low);
				result[ii + jj] = detail::FullAdder(result[ii + jj], low, c);
				result[ii + jj + 1] = detail::FullAdder(result[ii + jj + 1], high, c);
				if (c)
					++result[ii + jj + 2];
			}
		}
		if (result.back() == 0)
			result.pop_back();
		return UInteger(result);
	}

	std::string UInteger::ToString10() const
	{
#define TO_STRING_10_BIT_ONCE 8
#define TO_STRING_MOD_ONCE (static_cast<unsigned int>(std::pow(10, TO_STRING_10_BIT_ONCE)))

		std::string result;
		if (_number.back() == 0)
		{
			result.push_back('0');
			return result;
		}

		UInteger q = *this, mod, q_tmp;
		do
		{
			Devide(q, UInteger(TO_STRING_MOD_ONCE), mod, q_tmp);
			q = q_tmp;
			save_type temp = mod._number[0];
			if (q != (unsigned)0) {
				for (size_t ii = 0;ii < TO_STRING_10_BIT_ONCE;ii++) {
					result.push_back('0' + temp % 10);
					temp /= 10;
				}
			}
			else {
				size_t ii = 0;
				do {
					result.push_back('0' + temp % 10);
					temp /= 10;
				} while (ii < TO_STRING_10_BIT_ONCE && temp != 0);
			}
		} while (q != (unsigned)0);
		std::reverse(result.begin(), result.end());
		return result;
	}

	char NumberToChar(const unsigned ch)
	{
		if (ch < 10)
			return (static_cast<char>(ch) + '0');
		else if (ch >= 10 && ch <= 15)
			return (static_cast<char>(ch) + 'A' - 10);
		else
			throw std::logic_error("Wrong Number when convert number to char.");
	}

	std::string UInteger::ToString16() const
	{
		std::string result;

		if (_number.back() == 0)
		{
			result = "0x0";
			return result;
		}
		result.append("0x");

		auto it = _number.crbegin();
		save_type temp = *(it++);
		assert(temp != 0);
		unsigned int jj = 0;
		while (!(temp & ((save_type)15 << (BIT_NUMBER - 4))))
		{
			temp <<= 4;
			jj++;
		}
		for (;jj < 8;jj++)
		{
			result.push_back(
				NumberToChar(
					static_cast<unsigned int>(
					(temp & ((save_type)15 << (BIT_NUMBER - 4))) >> (BIT_NUMBER - 4)
						)));
			temp <<= 4;
		}
		for (;it != _number.crend();it++)
		{
			temp = *it;
			for (jj = 0;jj < 8;jj++)
			{
				result.push_back(
					NumberToChar(
						static_cast<unsigned int>(
						(temp & ((save_type)15 << (BIT_NUMBER - 4))) >> (BIT_NUMBER - 4)
							)));
				temp <<= 4;
			}
		}
		return result;
	}

	//除法
	void devide_Knuth(const vector<save_type>& src1, const vector<save_type>& src2,
		vector<save_type>& mod, vector<save_type>& quotient) {
		assert(src1.size() > src2.size() ||
			(src1.size() == src2.size() && src1.back() >= src2.back()));

		//规格化
		save_type temp = src2.back();
		int shift = 31;
		if ((temp & 0xffff0000) != 0) { shift -= 16; temp &= 0xffff0000; }
		if ((temp & 0xff00ff00) != 0) { shift -= 8; temp &= 0xff00ff00; }
		if ((temp & 0xf0f0f0f0) != 0) { shift -= 4; temp &= 0xf0f0f0f0; }
		if ((temp & 0xcccccccc) != 0) { shift -= 2; temp &= 0xcccccccc; }
		if ((temp & 0xaaaaaaaa) != 0) { shift -= 1; }
		vector<save_type> number1 = shift_left(src1, shift);
		const vector<save_type> number2 = shift_left(src2, shift);

		//除法
		if (number1.back() >= number2.back())
			number1.push_back(0);
		size_t size1 = number1.size(), size2 = number2.size();
		vector<save_type> result(size1 - size2);
		for (int ii = size1 - size2 - 1;ii >= 0;ii--)
		{
			//预测q
			calc_type_u qBar = number1[ii + size2] * MODULE + number1[ii + size2 - 1];
			calc_type_u rBar = qBar % number2.back();
			qBar /= number2.back(); // in this condition, qBar <= MODULE
			if (qBar == MODULE || qBar*number2[size2 - 2] > rBar * MODULE + number1[ii + size2 - 2])
				qBar--;

			//减法
			vector<save_type> tempvec = detail::multiply_vec(number2,
				vector<save_type>{static_cast<save_type>(qBar)});
			if (tempvec.size() == size2)
				tempvec.push_back(0);
			save_type c = 1;
			for (decltype(tempvec.size()) jj = 0;jj < tempvec.size();jj++)
				number1[jj + ii] = detail::FullSuber(number1[jj + ii], tempvec[jj], c);

			//如果预测不正确
			if (c == 0)
			{
				decltype(tempvec.size()) jj = 0;
				for (;jj < size2;jj++)
					number1[jj + ii] = detail::FullAdder(number1[jj + ii], number2[jj], c);
				if (jj < tempvec.size())
					number1[size2 + ii] = detail::FullAdder(number1[size2 + ii], 0, c);
				assert(c == 1);
				qBar--;
			}
			result[ii] = static_cast<save_type>(qBar);
		}

		//除去可能的高位的零
		detail::eraseZero(number1);
		mod = shift_right(number1, shift);
		quotient = result;
	}

	void devide_simple(const vector<save_type>& src1, const save_type src2,
		save_type& mod, vector<save_type>& quotient) {
		calc_type_u temp = 0;
		//save_type number2 = Obj2._number[0];
		auto size1 = src1.size();
		vector<save_type> result(size1);
		//vector<save_type> number1 = Obj1._number;
		//temp = 0;
		for (int ii = size1 - 1;ii >= 0;ii--)
		{
			temp *= MODULE;
			temp += src1[ii];
			result[ii] = static_cast<save_type>(temp / src2);
			temp = static_cast<calc_type_u>(temp % src2);
		}
		if (result.back() == 0 && result.size() > 1)
			result.pop_back();

		mod = static_cast<save_type>(temp);
		quotient = result;
	}

	//使用Knuth算法
	void Devide(const UInteger& Obj1, const UInteger& Obj2, UInteger& mod, UInteger& quotient)
	{
		assert(Obj2._number.back() != 0);
		//如果被除数小于除数，直接返回结果
		if (Obj1 < Obj2)
		{
			mod = Obj1;
			quotient = UInteger((unsigned)0);
			return;
		}

		//如果除数只有一位
		if (Obj2._number.size() < 2)
		{
			vector<save_type> quotient_v;
			save_type mod_n;

			devide_simple(Obj1._number, Obj2._number[0], mod_n, quotient_v);
			mod = UInteger(vector<save_type>{mod_n});
			quotient = UInteger(quotient_v);
		}
		else {
			vector<save_type> mod_v, quotient_v;

			devide_Knuth(Obj1._number, Obj2._number, mod_v, quotient_v);
			mod = UInteger(mod_v);
			quotient = UInteger(quotient_v);
		}
		return;
	}

	bool IsEven(const UInteger& num)
	{
		return !(bool)(num._number[0] & 1);
	}

	unsigned CharToNumber(const char& ch)
	{
		if (ch >= '0' && ch <= '9')
			return (ch - '0');
		else if (ch >= 'a' && ch <= 'f')
			return (ch - 'a' + 10);
		else
			return (ch - 'A' + 10);
	}

	void UInteger::FromString10(const std::string &c) {
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.cbegin(), end = c.cend();

		UInteger result;
		for (;it != end;it++)
		{
			result = (result << 3) + (result << 1);
			result = result + CharToNumber(*it);
		}
		result._number.swap(_number);
	}

	void UInteger::FromString2(const std::string &c) {
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.crbegin(), end = c.crend();

		vector<save_type> result;
		do {
			save_type temp = 0;
			for (int jj = 0;jj < 32 && it != end;++it, ++jj)
			{
				temp += (CharToNumber(*it) << jj);
			}
			result.push_back(temp);
		} while (it != end);
		_number.swap(result);
	}

	void UInteger::FromString8(const std::string &c) {
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.cbegin(), end = c.cend();

		UInteger result;
		for (;it != end;it++)
		{
			result = result << 3;
			result = result + CharToNumber(*it);
		}
		result._number.swap(_number);
	}

	void UInteger::FromString16(const std::string &c) {
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.crbegin(), end = c.crend();

		vector<save_type> result;
		do {
			save_type temp = 0;
			for (int jj = 0;jj < 8 && it != end;++it, ++jj)
			{
				temp += (CharToNumber(*it) << (4 * jj));
			}
			result.push_back(temp);
		} while (it != end);
		_number.swap(result);
	}

#define Number_UInteger_Parse_B 2
#define Number_UInteger_Parse_O 8
#define Number_UInteger_Parse_H 16
#define Number_UInteger_Parse_D 4

#define EXPECT(it, ch) do{ \
if (*(it++) != (ch) \
	throw std::runtime_error("Invalid char."); \
}

	bool IsCharB(char c) {
		return (c >= '0' && c <= '1');
	}

	bool IsCharO(char c) {
		return (c >= '0' && c <= '7');
	}

	bool IsCharD(char c) {
		return (c >= '0' && c <= '9');
	}

	bool IsCharH(char c) {
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
	}

	int IntegerParseNumber(std::string::const_iterator &SrcIt, std::string &c, decltype(IsCharB) fun, int Ret) {
		if (!fun(*SrcIt))
			throw std::runtime_error("Invalid char.");
		//跳过前导零
		while (*SrcIt == '0')
			++SrcIt;
		if (!fun(*SrcIt))
			--SrcIt;

		c.clear();
		while (fun(*SrcIt)) {
			c.push_back(*(SrcIt++));
		}
		return Ret;
	}

	int IntegerParse2(std::string::const_iterator &SrcIt, std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharB, Number_UInteger_Parse_B);
	}

	int IntegerParse8(std::string::const_iterator &SrcIt, std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharO, Number_UInteger_Parse_O);
	}

	int IntegerParse16(std::string::const_iterator &SrcIt, std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharH, Number_UInteger_Parse_H);
	}

	int IntegerParse10(std::string::const_iterator &SrcIt, std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharD, Number_UInteger_Parse_D);
	}

	int IntegerParseZero(std::string::const_iterator &SrcIt, std::string &c) {
		switch (*SrcIt) {
		case 'b': case 'B': return IntegerParse2(++SrcIt, c);
		case 'o': case 'O': return IntegerParse8(++SrcIt, c);
		case 'x': case 'X': return IntegerParse16(++SrcIt, c);
		default:
			std::string c = "0";
			return Number_UInteger_Parse_D;
		}
	}

	int IntegerParseValue(std::string::const_iterator &SrcIt, std::string &c) {

		switch (*SrcIt) {
		case '0': return IntegerParseZero(++SrcIt, c);
		default:  return IntegerParse10(SrcIt, c);
		}
	}

	int UInteger::_parse(std::string::const_iterator & it)
	{
		std::string c;

		int res = IntegerParseValue(it, c);

		if (*it != '\0')
			return Number_Parse_Failed;

		switch (res) {
		case Number_UInteger_Parse_B:  this->FromString2(c);  break;
		case Number_UInteger_Parse_O:  this->FromString8(c);  break;
		case Number_UInteger_Parse_D:  this->FromString10(c); break;
		case Number_UInteger_Parse_H:  this->FromString16(c); break;
		}
		return Number_Parse_OK;
	}

	//从字符串输入数值
	//支持2，8，10，16进制
	//词法规则：((0b num2 num2*)|(0o num8 num8*)|(0x num16 num16*)|(num10 num10*))
	int UInteger::Parse(std::string str)
	{
		try {
			if (str.empty() || str.back() != '\0')
				str.push_back('\0');
			auto it = str.cbegin();
			return _parse(it);
		}
		catch (std::exception)
		{
			return Number_Parse_Failed;
		}
	}

	std::ostream & operator<<(std::ostream& os, const UInteger & Obj)
	{
		// TODO: 在此处插入 return 语句
		return (os << Obj.ToString10());
	}

	std::istream & operator>>(std::istream& is, UInteger &Num)
	{
		// TODO: 在此处插入 return 语句
		std::string str;
		is >> str;
		Num.Parse(str);
		return is;
	}

	size_t UInteger::size() const
	{
		return _number.size();
	}


} // namespace Number
