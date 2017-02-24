#include "Integer.h"
#include <algorithm>

using std::vector;
using std::string;

namespace Number {

	Integer::Integer(const vector<save_type>& Number, char Signal) 
		: _number(Number), _signal(Signal) {}

	Integer::Integer(vector<save_type>&& Number, char Signal)
		: _number(Number), _signal(Signal) {};

	Integer::Integer(const Integer& src) : _number(src._number), _signal(src._signal) {}

	Integer::Integer(const UInteger & Number, const char Signal)
		: _number(Number), _signal(Signal > 0 ? 1 : -1) {}

	Integer & Integer::operator=(const Integer &src)
	{
		_number = src._number;
		_signal = src._signal;
		return *this;
	}

	Integer & Integer::operator=(Integer &&src)
	{
		_number = std::forward<UInteger>(src._number);
		_signal = src._signal;
		return *this;
	}

	int Integer::setSignal(const int& signal)
	{
		return _signal = signal >= 0 ? 1 : -1;
	}

	int Integer::Compare(const Integer &Obj2) const
	{
		if (_numvec.back() == 0 && Obj2._numvec.back() == 0)
			return 0;
		//符号不同直接判断
		else if (_signal != Obj2._signal)
			return static_cast<int>(_signal - Obj2._signal);
		else {
			auto res = _number.Compare(Obj2._number);
			return _signal > 0 ? res : -res;
		}
	}

	Integer Integer::operator<<(int num) const
	{
		UInteger result;
		result << num;
		return Integer(result, _signal);
	}

	Integer Integer::operator>>(int num) const {
		UInteger result;
		result >> num;
		return Integer(result, _signal);
	}

	// Minus
	Integer Integer::operator-() const {
		return Integer(_number, -_signal);
	}
	// Add
	Integer Integer::Add(const Integer& Obj2) const
	{
		//若两数符号不同，则实际为减法
		if (this->_signal != Obj2._signal)
			return this->Sub(-Obj2);
		return Integer(_number + Obj2._number, _signal);
	}

	Integer Integer::Sub(const Integer& Obj2) const	{
		//若两数符号不同，则实际为加法
		if (this->_signal != Obj2._signal)
			return Integer(_number + Obj2._number, _signal);

		if (_number < Obj2._number)
			return Integer(Obj2._number.Sub(_number), -_signal);
		else
			return Integer(_number.Sub(Obj2._number), _signal);
	}

	Integer Integer::Multiply(const Integer& Obj2) const {
		//signal
		auto signal = this->_signal * Obj2._signal;
		return Integer(_number.Multiply(Obj2._number), signal);
	}

	void Devide(const Integer& Obj1, const Integer& Obj2, Integer& mod, Integer& quotient)
	{
		Devide(Obj1._number, Obj2._number, mod._number, quotient._number);
		quotient._signal = Obj1._signal * Obj2._signal;
		mod._signal = Obj1._signal;
	}

	std::ostream & operator<<(std::ostream & os, const Integer & Obj)
	{
		// TODO: 在此处插入 return 语句
		return (os << Obj.ToString10());
	}

	std::istream & operator>>(std::istream &is, Integer &Num)
	{
		// TODO: 在此处插入 return 语句
		std::string str;
		is >> str;
		Num.Parse(str);
		return is;
	}

	string Integer::ToString10() const {
		string result;
		if (_signal < 0 && _numvec.back() != 0)
			result = '-' + _number.ToString10();
		else
			result = _number.ToString10();
		return result;
	}

	std::string Integer::ToString16() const
	{
		string result;
		if (_signal < 0 && _numvec.back() != 0)
			result = '-' + _number.ToString16();
		else
			result = _number.ToString16();
		return result;
	}

	//从字符串输入数值
	//支持2，8，10，16进制
	//词法规则：(+|-|ε)((0b num2 num2*)|(0o num8 num8*)|(0x num16 num16*)|(num10 num10*))
	int Integer::Parse(std::string str)
	{
		if (str.empty() || str.back() != '\0')
			str.push_back('\0');
		auto it = str.cbegin();
		try {
			//处理符号
			detail::NumberParseSignal(it, _signal);
			return _number._parse(it);
		}
		catch (std::exception) {
			return Number_Parse_Failed;
		}
	}

	size_t Integer::size() const
	{
		return _number._number.size();
	}

	UInteger Integer::Abs() const
	{
		return _number;
	}

	Integer Integer::Power(save_type Exp) const {
		return Integer(_number.Power(Exp), (Exp & 1) == 0 ? 1 : _signal);
	}

	Integer Integer::Power(const UInteger & Exp) const {
		return Integer(_number.Power(Exp), (Exp._number[0] & 1) == 0 ? 1 : _signal);
	}

}
