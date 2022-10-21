#include <iostream>
#include <type_traits>

// constexpr variable is const, and requires can be evaluated at compile time
// if you do not want get a const variable but want it initialized at compile time, use constinit since C++20

// use constinit avoid the undefined initialization order of static/global variables.
// and because it's mutable, it can not be used at compile-time const expression.

template<size_t N>
constexpr int Fibonacci = Fibonacci<N-1> + Fibonacci<N-2>;
template<>
constexpr int Fibonacci<0> = 0;
template<>
constexpr int Fibonacci<1> = 1;

constinit int a = Fibonacci<10>; // will be initialized at compile time, only used on static lifetime variable

int main(int argc, char const *argv[])
{
    std::cout << a << std::endl;
    return 0;
}
