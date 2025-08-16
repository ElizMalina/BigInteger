# BigInteger

![I can't upload a theme image](images/image.png)



During our second semester at St. Petersburg State University, we were tasked with writing a class called "BigInteger" to represent long integers (signed numbers). The project was written in C++.



The class implements:
 - The default constructor.
 - Constructors from numeric types (int, unsigned int, long, unsigned long, long long, unsigned long long).
 - An explicit constructor from the BigInteger(std::string, biginteger_base base=biginteger_base::dec) string, which supports the 10-ary and 16-ary number systems.
 - The copy constructor.
 - The move constructor.
 - Operators:
   - Assignment by copy,
   - assignment by moving,
   - comparisons.
 - Arithmetic operations: 
   - addition, 
   - subtraction, 
   - multiplication, 
   - division, 
   - unary minus, 
   - unary plus, 
   - increments and decrements.
 - The external function std::string to_string(const BigInteger &).
 - The external function std::string to_hex(const BigInteger &) to represent a number as a string in the 16th base.