#include <iostream>

// compile-time dump, only works on clang, g++ will not show detail type info
// do not define for gcc, it will print the types
template<typename...>
struct [[deprecated]] dump {};

// run-time dump
template<typename...>
void rtdump()
{
    std::cout << __func__ << std::endl;
    std::cout << __FUNCTION__ << std::endl;
    std::cout << __PRETTY_FUNCTION__ << std::endl; // GCC feature
}

// other: https://cppinsights.io/

int main(int argc, char const *argv[])
{
    int a;
    int* b;
    int& ra = a;
    dump<decltype(a), decltype(b), decltype(ra), decltype((a)), decltype((b)), decltype(std::move(a))>();
    rtdump<decltype(a), decltype(b), decltype(ra), decltype((a)), decltype((b)), decltype(std::move(a))>();
    return 0;
}
