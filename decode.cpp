#include "biginteger.hpp"
#include <iostream>
#include <fstream>

std::string string_parsing(std::string line);

BigInteger SRT(const BigInteger& mes, const BigInteger& d, const BigInteger& p, const BigInteger& q);

int main(int argc, char* argv[]) {
    const char* input_1FileName = argv[1];
    const char* input_2FileName = argv[2];

    std::ifstream keyfile(input_1FileName);
    if (!keyfile) {
        std::cerr << "Can not open 'keyfile' \n";
        return 0;
    }

    std::ifstream mesfile(input_2FileName);
    if (!mesfile) {
        std::cerr << "Can not open 'mesfile' \n";
        return 0;
    }

    std::string line;

    std::string n; // modulus
    std::string e; 
    std::string d; // private exponent
    std::string p; // prime1
    std::string q; // prime2

    std::string mes;

    while (std::getline(keyfile, line)) {
        if (line[0] == 'm') {
            n = string_parsing(line);
        }
        else if (line[1] == 'u') {
            e = string_parsing(line);
        }
        else if (line[3] == 'v') {
            d = string_parsing(line);
        }
        else if (line[5] == '1') {
            p = string_parsing(line);
        }
        else if (line[5] == '2') {
            q = string_parsing(line);
        }
    }

    int ch;
    while ((ch = mesfile.get()) != EOF) {
        mes += static_cast<char>(ch);
    }

    
    BigInteger c(mes, biginteger_base::hex);
    BigInteger D(d, biginteger_base::hex);
    BigInteger P(p, biginteger_base::hex);
    BigInteger Q(q, biginteger_base::hex);

    std::cout << to_hex(SRT(c, D, P, Q));
    
    keyfile.close();
    mesfile.close();

    return 0;
}

std::string string_parsing(std::string line) {
    bool colon = false;
    std::string res;

    for (std::size_t i = 0; i < line.size(); ++i) {
        if (colon) { //когда мы находимся после двоеточия
            if (line[i] != ' ') {
                res += line[i];
            }
        }
        else if (line[i] == ':') {
            colon = true;
        }
    }

    return res;
}


BigInteger mod(const BigInteger& x, const BigInteger& y) {
    const BigInteger& n = y;

    if (n<= 0) {
        throw std::invalid_argument("Negative modulus");
    }

    BigInteger res = x % y;
    if (res < 0) {
        return res + y;
    }
    return res;
}

BigInteger RSA(BigInteger mes, BigInteger d, const BigInteger& n) {
    if (n == 1) {
        return 0;
    }

    BigInteger res = 1;
    mes %= n;

    while (!d.isequal(0)) {
        if ((d % 2).isequal(1)) {
            res *= mes;
            res %= n;
        }

        mes *= mes;
        mes %= n;

        d /= 2;
    }

    return res;
}

BigInteger reverse_mod(BigInteger x, BigInteger y) {
    BigInteger y_0 = y;
    BigInteger a = 0;
    BigInteger b = 1;

    bool negative = false;

    if (y == 1) {
        return 0;
    }

    while (x > 1) {
        BigInteger q = x / y;
        BigInteger temp = y;

        y = x % y;
        x = temp;

        temp = a;
        a = b - q * a;
        b = temp;
    }

    if (b < 0) {
        b += y_0;
    }

    return b;
}
//китайская теорема об остатках для ускоренного RSA-дешифрования

BigInteger SRT(const BigInteger& mes, const BigInteger& d, const BigInteger& p, const BigInteger& q) {
    BigInteger dp = d % (p - 1);
    BigInteger dq = d % (q - 1);

    BigInteger qInv = reverse_mod(q, p);  // q^-1 mod p

    BigInteger m1 = RSA(mes, dp, p);      // mes^dp mod p
    BigInteger m2 = RSA(mes, dq, q);      // mes^dq mod q

    BigInteger h = mod(qInv * (m1 - m2), p);  // Ensure h is non-negative

    return m2 + h * q;
}