# FeNAl
Flags about excited numerical algorithm.

## Contents now
- Multiprecision class for unsigned integer : UInteger.
- Multiprecision class for signed integer : Integer.
- Multiprecision class for floating point number : Real.
- Basic algorithm : fast power of integer(signed and unsigned).
- Basic algorithm : adaptive simpson integration method for any function.
- Class for polynomial and numerical method to find all the root.
- Tests for these method above.
- Remainder-theorem implementation.

## Requirements
- This project using c++17 charactoristics.
- CMake version >= 3.8.0
- VS2015 rc2 or later; GCC 6.1 or later; Clang 4.0 or later.

## Build
- Make a new directories "build" in the FeNAl folder and enter.
- Run command "cmake ..".
- Run command "make & make test" if you are in Linux;
- Build project using VS if you are in Windows.

## ToDo:
- Add tests for Remainder-theorem.
- Optimize the performance. Make it faster.
- Multiply number using FFT or so.
- Constructing expresion template for UInteger, Integer, Real.
- Class for general tensor.
- Template for quantity with dimension.
- Special function calculate, for example legendre, bessel, supergeometric and so on.
- and so on.
