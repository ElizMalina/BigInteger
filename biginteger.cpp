#include "biginteger.hpp"
#include <string>
#include <iostream>

BigInteger::BigInteger() : size_number(1), sign(1) {
	Number = new std::uint32_t[1];
	Number[0] = 0;
	Normalize();
}
BigInteger::BigInteger(int num) : size_number(1) {
	sign = (num >= 0) ? 1 : -1;
	std::uint32_t modul = (sign == 1) ? num : -num;
	Number = new std::uint32_t[1];
	Number[0] = modul;
	Normalize();
}
BigInteger::BigInteger(unsigned int num) : size_number(1), sign(1) {
	Number = new std::uint32_t[1];
	Number[0] = num;
	Normalize();
}
BigInteger::BigInteger(long num) {
	sign = (num >= 0) ? 1 : -1;
	std::uint32_t modul = (sign == 1) ? static_cast<std::uint32_t>(num) : static_cast<std::uint32_t>(-num);
	if (num <= (two_in_32 - 1)) {
		size_number = 1;
		Number = new std::uint32_t[1];
		Number[0] = modul;
	}
	else {
		size_number = 2;
		Number = new std::uint32_t[2];
		for (int i = 0; i < 2; ++i) {
			Number[0] = static_cast<std::uint32_t>(modul % two_in_32);
			Number[1] = static_cast<std::uint32_t>(modul / two_in_32);
		}
	}
	Normalize();
}
BigInteger::BigInteger(unsigned long num) : sign(1) {
	if (num <= (two_in_32 - 1)) {
		size_number = 1;
		Number = new std::uint32_t[1];
		Number[0] = static_cast<std::uint32_t>(num);
	}
	else {
		size_number = 2;
		Number = new std::uint32_t[2];
		for (int i = 0; i < 2; ++i) {
			Number[0] = static_cast<std::uint32_t>(num % two_in_32);
			Number[1] = static_cast<std::uint32_t>(num / two_in_32);
		}
	}
	Normalize();
}
BigInteger::BigInteger(long long num) {
	sign = (num >= 0) ? 1 : -1;
	std::uint64_t modul = static_cast<std::uint64_t>(num * sign);

	if (modul <= (two_in_32 - 1)) {
		size_number = 1;
		Number = new std::uint32_t[1];
		Number[0] = static_cast<uint32_t>(modul);
	}
	else {
		size_number = 2;
		Number = new std::uint32_t[2];
		for (int i = 0; i < 2; ++i) {
			Number[0] = static_cast<std::uint32_t>(modul % two_in_32);
			Number[1] = static_cast<std::uint32_t>(modul / two_in_32);
		}
	}
	Normalize();
}
BigInteger::BigInteger(unsigned long long num) : sign(1) {
	if (num <= (two_in_32 - 1)) {
		size_number = 1;
		Number = new std::uint32_t[1];
		Number[0] = static_cast<std::uint32_t>(num);
	}
	else {
		size_number = 2;
		Number = new std::uint32_t[2];
		for (int i = 0; i < 2; ++i) {
			Number[0] = static_cast<std::uint32_t>(num % two_in_32);
			Number[1] = static_cast<std::uint32_t>(num / two_in_32);
		}
	}
	Normalize();
}

bool BigInteger::isequal(const BigInteger& y) const {
	bool flag;
	flag = ((sign == y.sign) && (size_number == y.size_number));
	if (flag) {
		for (int i = 0; i < size_number; ++i) {
			if (!(Number[i] == y.Number[i])) {
				flag = false;
				break;
			}
		}
	}
	return flag;
}
bool BigInteger::isless(const BigInteger& x) const {
	if (sign != x.sign) {
		return sign < x.sign;
	}
	if (sign == 1) {
		if (size_number != x.size_number) {
			return size_number < x.size_number;
		}
		for (int i = static_cast<int>(size_number - 1); i >= 0; --i) {
			if (Number[i] != x.Number[i]) {
				return Number[i] < x.Number[i];
			}
		}
		return false;
	}
	if (size_number != x.size_number) {
		return size_number > x.size_number;
	}
	for (int i = static_cast<int>(size_number - 1); i >= 0; --i) {
		if (Number[i] != x.Number[i]) {
			return Number[i] > x.Number[i];
		}
	}
	return false;
}
bool operator==(const BigInteger& x, const BigInteger& y)
{
	return x.isequal(y);
}
bool operator!=(const BigInteger& x, const BigInteger& y) {
	return !(x == y);
}
bool operator<(const BigInteger& x, const BigInteger& y) {
	return x.isless(y);
}
bool operator<=(const BigInteger& x, const BigInteger& y) {
	return (x.isless(y) || x.isequal(y));
}
bool operator>(const BigInteger& x, const BigInteger& y) {
	return !(x<=y);
}
bool operator>=(const BigInteger& x, const BigInteger& y) {
	return !(x<y);
} 

BigInteger operator*(const BigInteger& x, const BigInteger& y) {
	return BigInteger::Multiplication(x, y);
}

BigInteger operator+(const BigInteger& x, const BigInteger& y) { 
	BigInteger tmp = x;
	tmp += y;
	return tmp;
}

BigInteger operator-(const BigInteger& x, const BigInteger& y)
{
	BigInteger res = x;
	res -= y;
	return res ;
}

BigInteger BigInteger::Division(const BigInteger& x, const BigInteger& y, BigInteger& R) {
	BigInteger res;
	R = 0;
	for (int i = static_cast<int>(x.size_number) - 1; i >= 0; --i) {
		R *= two_in_32;
		R += x.Number[i];
		res *= two_in_32;
		uint32_t l = 0;
		uint32_t r = two_in_32 - 1;
		uint32_t q = 0;
		while (l <= r) {
			uint32_t M = l + ((r - l) >> 1);
			BigInteger M_Y = y * M;
			if (M_Y <= R) {
				q = M;
				l = M + 1;
			}
			else { r = M - 1; }
		}
		R -= y * q;
		res += q;
	}
	return res;
}

BigInteger operator/(const BigInteger& x, const BigInteger& y)
{
	BigInteger res = x;
	res /= y;
	return res;
}

BigInteger operator%(const BigInteger& x, const BigInteger& y)
{
	BigInteger res = x;
	res %= y;
	return res;
}

BigInteger::BigInteger(const std::string& stroka, biginteger_base base) {
	std::string str;

	for (char ch : stroka) {
		if (ch != ' ' && ch != '\n') {
			str += ch;
		}
	}
	//std::cout << str;
	sign = 1;
	if (str[0] == '-') {
		sign = -1;
		str.erase(0, 1);
	}
	else if(str[0] == '+') {
		str.erase(0, 1);
	}
	if (str.empty()) {
		throw std::invalid_argument("ERROR!!!!!!!!!!");
	}
	if (base == biginteger_base::hex) {
		std::string result = "0";
		for (char ch : str) {
			int digits;

			if (ch >= '0' && ch <= '9') {
				digits = ch - '0';
			}
			else if (ch >= 'A' && ch <= 'F') {
				digits = ch - 'A' + 10;
			}
			else if (ch >= 'a' && ch <= 'f') {
				digits = ch - 'a' + 10;
			}
			else {
				throw std::invalid_argument("The argument hex is incorrect");
			}

			result = Summa(Multiplication(result, std::to_string(16)), std::to_string(digits)); // TODO функции по сумме и произведению строк 
		}
		str = result;
	}

	size_number = 1;
	Number = new std::uint32_t[1]{ 0 };

	const std::uint32_t BASE = 10;

	auto multiplication = [this](std::uint32_t multiplier) {
		std::uint64_t carry = 0;

		for (size_t i = 0; i < size_number; ++i) {
			std::uint64_t res = static_cast<std::uint64_t>(Number[i]) * multiplier + carry;
			Number[i] = static_cast<std::uint32_t>(res % two_in_32);
			carry = res / two_in_32;
		}

		if (carry > 0) {
			auto* new_num = new std::uint32_t[size_number + 1];
			for (size_t i = 0; i < size_number; ++i) {
				new_num[i] = Number[i];
			}
			new_num[size_number] = static_cast<std::uint32_t>(carry);
			delete[] Number;
			Number = new_num;
			++size_number;
		}
		};

	auto add_digit = [this](std::uint32_t digit) {
		std::uint64_t carry = digit;

		for (size_t i = 0; i < size_number; ++i) {
			std::uint64_t sum = static_cast<std::uint64_t>(Number[i]) + carry;
			Number[i] = static_cast<std::uint32_t>(sum % two_in_32);
			carry = sum / two_in_32;
			if (carry == 0) {
				break;
			}
		}

		if (carry > 0) {
			auto* new_num = new std::uint32_t[size_number + 1];
			for (size_t i = 0; i < size_number; ++i) {
				new_num[i] = Number[i];
			}
			new_num[size_number] = static_cast<std::uint32_t>(carry);
			delete[] Number;
			Number = new_num;
			++size_number;
		}
		};

	
	for (char ch : str) {
		std::uint32_t digit;
		if (!(ch >= '0' && ch <= '9')) {
			clear();
			throw std::invalid_argument("Error");
		}
		digit = ch - '0';

		multiplication(BASE);
		add_digit(digit);
	}
}
//перевод в строку 10с.с

std::string BigInteger::to_string() const {
	BigInteger copy(*this);
	std::string result;
	copy.sign = 1;

	if (copy.size_number == 1 && copy.Number[0] == 0) {
		return "0";
	}

	const std::uint64_t BASE = 10;

	while (copy.size_number != 1 || copy.Number[0] != 0) {
		std::uint64_t ost = 0;

		for (int i = static_cast<int>(copy.size_number - 1); i >= 0; --i) {
			std::uint64_t current = ost * two_in_32 + copy.Number[i];
			copy.Number[i] = static_cast<uint32_t>(current / BASE);
			ost = current % BASE;
		}

		while (copy.size_number > 1 && copy.Number[copy.size_number - 1] == 0) {
			--copy.size_number;
		}

		result += static_cast<char>('0' + ost);

	}

	if (sign == -1) {
		result += '-';
	}

	for (int i = 0, j = static_cast<int>(result.size() - 1); i < j; ++i, --j) {
		std::swap(result[i], result[j]);
	}

	return result;

}

std::string to_string(const BigInteger& x) {
	return x.to_string();
}

std::string to_hex(const BigInteger& x) {
	std::string dec = to_string(x);
	std::string res{};

	if (dec == "0") {
		return "0";
	}
	while (dec != "0") {
		BigInteger num(dec);
		int num_16 = stoi(to_string(num % 16 ));
		char digit{};
		if (num_16 >= 0 && num_16 <= 9) {
			digit = static_cast<char>(num_16+'0');
		}
		else if (num_16 >= 10 && num_16 <= 15) {
			digit = static_cast<char>(num_16 + 'A' - 10);
		}
		else {
			throw std::invalid_argument("ERROR");
		}
		res.push_back(digit);
		dec = to_string(num / 16);
	}

	int left = 0;
	int right = static_cast<int>(res.size() - 1);
	while (left < right) {
		std::swap(res[left], res[right]);
		++left;
		--right;
	}

	return res;
}