#ifndef BIG_INTEGER_HPP
#define BIG_INTEGER_HPP
#include <string>
#include <iostream>

enum class biginteger_base { dec = 10, hex = 16 };
constexpr uint64_t two_in_32 = 4294967296;

class BigInteger {
private:
	std::uint32_t* Number;
	std::size_t size_number;
	int sign;

public:
	//конструктор по умолчанию, инициализирует число нулем

	void Normalize() {
		while (size_number > 1 && Number[size_number - 1] == 0) {
			--size_number; 
		}
		if (size_number == 1 && Number[0] == 0) { 
			sign = 1; 
		}
	}

	BigInteger();
	BigInteger(int num);
	BigInteger(unsigned int num);
	BigInteger(long num);
	BigInteger(unsigned long num);
	BigInteger(long long num);
	BigInteger(unsigned long long num);
	BigInteger(const std::string& stroka, biginteger_base base = biginteger_base::dec);
	// конструктор копирования

	BigInteger(const BigInteger& num):sign(num.sign), size_number(num.size_number) { 
		Number = new std::uint32_t[size_number];
		for (int i = 0; i < size_number; ++i) {
			Number[i] = num.Number[i];
		}
	} 
	// конструктор перемещения

	BigInteger(BigInteger&& num) noexcept : sign(num.sign), size_number(num.size_number),Number(num.Number) { 
		num.sign = 1;
		num.size_number = 0;
		num.Number = nullptr;
	}

	// присваивание копированием

	BigInteger& operator=(const BigInteger& other) { 
		if (this != &other) {
			delete[] Number;
			size_number = other.size_number;
			sign = other.sign;
			Number = new std::uint32_t[size_number];
			for (int i = 0; i < size_number; ++i) {
				Number[i] = other.Number[i];
			}
		}
		return *this;
	}
	// присваивание перемещением

	BigInteger& operator=(BigInteger&& other) noexcept { 
		if (this != &other) {
			delete[] Number;
			size_number = other.size_number;
			sign = other.sign;
			Number = other.Number;

			other.size_number = 0;
			other.sign = 1;
			other.Number = nullptr;
		}
		return *this;
	}

	//bool operator==(const BigInteger& other) const;
	void clear() {
		size_number = 0;
		sign = 1;
		delete[] Number;
	}
	 //умножение

	static BigInteger Multiplication(const BigInteger& x, const BigInteger& y) {
		BigInteger res;
		res.clear();
		res.size_number = x.size_number + y.size_number;
		res.Number = new std::uint32_t[res.size_number]{};
		res.sign = (x.sign == y.sign) ? 1 : -1;
		for (size_t i = 0; i < x.size_number; ++i) {
			std::uint64_t carry = 0;
			for (size_t j = 0; j < y.size_number; ++j) {
				std::uint64_t Mult = static_cast<std::uint64_t>(x.Number[i]) * y.Number[j] + res.Number[i + j] + carry;
				res.Number[i + j] = static_cast<std::uint32_t>(Mult % two_in_32);
				carry = Mult / two_in_32;
			}
			res.Number[i + y.size_number] = static_cast<std::uint32_t>(carry);
		}

		while (res.size_number > 1 && res.Number[res.size_number - 1] == 0) {
			--res.size_number;
		}

		return res;
	}

	static BigInteger Summa(const BigInteger& x, const BigInteger& y) {
		BigInteger res;
		res.clear();
		res.size_number = std::max(x.size_number, y.size_number) + 1;
		res.Number = new std::uint32_t[res.size_number]{};

		std::uint64_t carry = 0;
		for (size_t i = 0; i < res.size_number - 1; ++i) {
			std::uint64_t xi = (i < x.size_number) ? x.Number[i] : 0;
			std::uint64_t yi = (i < y.size_number) ? y.Number[i] : 0;

			std::uint64_t sum = xi + yi + carry;
			res.Number[i] = static_cast<std::uint32_t>(sum);
			carry = sum >> 32;
		}

		res.Number[res.size_number - 1] = static_cast<std::uint32_t>(carry);

		
		while (res.size_number > 1 && res.Number[res.size_number - 1] == 0) {
			--res.size_number;
		}

		return res;

	}

	static std::string Multiplication(const std::string& x, const std::string& y) {
		if (x == "0" || y == "0") {
			return "0";
		}

		size_t sizex = x.size();
		size_t sizey = y.size();
		std::string res(sizex + sizey, '0');

		for (int i = sizex - 1; i >= 0; --i) {
			int xi = x[i] - '0';
			int carry = 0;
			for (int j = sizey - 1; j >= 0; --j) {
				int yi = y[j] - '0';
				int pos = i + j + 1;

				int sum = (res[pos] - '0') + xi * yi + carry;
				res[pos] = (sum % 10) + '0';
				carry = sum / 10;
			}
			res[i] += carry;
		}

		size_t start = res.find_first_not_of('0');
		return res.substr(start);
	}

	static std::string Summa(const std::string& x, const std::string& y) {
		int i = static_cast<int>(x.size()) - 1;
		int j = static_cast<int>(y.size()) - 1;
		int carry = 0;
		std::string res;

		while (i >= 0 || j >= 0 || carry) {
			int x_i = (i >= 0) ? x[i--] - '0' : 0;
			int y_i = (j >= 0) ? y[j--] - '0' : 0;
			int sum = x_i + y_i + carry;
			res.push_back((sum % 10) + '0');
			carry = sum / 10;
		}

		std::reverse(res.begin(), res.end());
		return res;

	}
	
	static BigInteger Different(const BigInteger& x, const BigInteger& y) {
		size_t resultLength = x.size_number;
		uint32_t* tempResult = new uint32_t[resultLength];
		size_t actualLength = 0;

		int64_t carry = 0;

		for (size_t idx = 0; idx < resultLength; ++idx) {
			int64_t val1 = static_cast<int64_t>(x.Number[idx]);
			int64_t val2 = (idx < y.size_number) ? static_cast<int64_t>(y.Number[idx]) : 0;

			int64_t difference = val1 - val2 - carry;

			if (difference < 0) {
				difference += static_cast<int64_t>(two_in_32);
				carry = 1;
			}
			else {
				carry = 0;
			}

			tempResult[actualLength++] = static_cast<uint32_t>(difference);
		}

		while (actualLength > 1 && tempResult[actualLength - 1] == 0) {
			--actualLength;
		}

		BigInteger result;
		result.clear();
		result.size_number = actualLength;
		result.Number = new uint32_t[actualLength];
		std::copy(tempResult, tempResult + actualLength, result.Number);
		result.sign = 1;

		delete[] tempResult;
		return result;
	}

	static BigInteger Division(const BigInteger& x, const BigInteger& y, BigInteger& R);
	

	BigInteger& operator-=(const BigInteger& other) {
		*this += -other;
	    return *this;
	}
	BigInteger& operator+=(const BigInteger& other) {
		if (sign == other.sign) {
			sign = true;
			BigInteger Other = other;
			Other.sign = true;
			*this = Summa(*this, Other);
			sign = other.sign;
		}
		else {
			int TMPSign = sign;
			BigInteger This = *this;
			BigInteger Other = other;
			This.sign = 1;
			Other.sign = 1;

			if (This.isless(Other)) {
				*this = Different(Other, This);
				
				sign = other.sign;
			}
			else if (Other.isless(This)) {
				*this = Different(This,Other);
				sign = TMPSign;
			}
			else {
				*this = 0;
			}
		}
		
		return *this;
	}
	BigInteger& operator*=(const BigInteger& other) {
		*this = Multiplication(*this, other);
		return *this;
	}
	BigInteger& operator/=(const BigInteger& other) {
		if (other.isequal(0)) {
			throw std::invalid_argument("Div by 0 in operator/=");
		}
		bool resSign = (sign == other.sign);

		BigInteger a = *this;
		BigInteger b = other;
		a.sign = true;
		b.sign = true;

		if (a.isless(b)) {
			*this = 0;
			return *this;
		}

		BigInteger tmp;
		*this = Division(a, b, tmp);
		sign = (resSign) ? 1 : -1;

		return *this;
	}

	BigInteger& operator%=(const BigInteger& other) {
		if (other.isequal(0)) {
			throw std::invalid_argument("Div by 0 in operator%=");
		}

		int resSign = sign;

		BigInteger a = *this;
		BigInteger b = other;
		a.sign = 1;
		b.sign = 1;

		if (a.isless(b)) {
			a.sign = resSign;
			*this = a;
			return *this;
		}

		BigInteger tmp;
		Division(a, b, tmp);
		if (tmp.isequal(0)) {
			tmp.sign = 1;
		}
		else {
			tmp.sign = resSign;
		}
		*this = tmp;
		return *this;
	}
	BigInteger operator -() const {
		BigInteger tmp = *this;
		if (!tmp.isequal(0)) { tmp.sign = (-1) * sign; }
		return tmp;
	}
	BigInteger operator +() const {
		BigInteger tmp = *this;
		return tmp;
	}

	BigInteger& operator++() {
		*this += 1;
		return *this;
	}
	BigInteger& operator--() {
		*this -= 1;
		return *this;
	}

	BigInteger operator++(int) {
		BigInteger tmp = *this;
		*this += 1;
		return tmp;
	}
	BigInteger operator--(int) {
		BigInteger tmp = *this;
		*this -= 1;
		return tmp;
	}

	bool isequal(const BigInteger& y) const;
	bool isless(const BigInteger& x) const;
	std::string to_string() const;
	

	~BigInteger() { clear(); }

};

bool operator==(const BigInteger& x, const BigInteger& y);
bool operator!=(const BigInteger& x, const BigInteger& y);

bool operator<(const BigInteger& x, const BigInteger& y);
bool operator<=(const BigInteger& x, const BigInteger& y);
bool operator>(const BigInteger& x, const BigInteger& y);
bool operator>=(const BigInteger& x, const BigInteger& y);

BigInteger operator*(const BigInteger& x, const BigInteger& y);
BigInteger operator+(const BigInteger& x, const BigInteger& y);
BigInteger operator-(const BigInteger& x, const BigInteger& y);
BigInteger operator/(const BigInteger& x, const BigInteger& y);
BigInteger operator%(const BigInteger& x, const BigInteger& y);
std::string to_string(const BigInteger& x);

std::string to_hex(const BigInteger& x); //представления числа в виде строки в 16-й системе счисления.


#endif