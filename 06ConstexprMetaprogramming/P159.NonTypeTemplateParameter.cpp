#include <iostream>
#include <type_traits>
#include <algorithm>

// C++20: auto as type of non-type template paramter, float point non-type template parameter, user-defined literal(constexpr) type non-type template paramter

struct Foo {};

template<auto...> struct ValueList {};

ValueList<'C', 0.2, 1L, Foo{}> x;

// we just can not use C-style string literal as non-type template arguments
// but there are work arounds:
//      globl char array, but it's fixed size
//      wrap C style string in literal type
template<size_t N>
struct FixedString
{
    char str[N];
    constexpr FixedString(const char (&s)[N])
    {
        std::copy_n(s, N, str);
    }
};

ValueList<FixedString("hello")> y;

// use literal operator for simplification
template<FixedString str>
constexpr decltype(str) operator""_fs() { return str; }

ValueList<"hello"_fs> z;

int main(int argc, char const *argv[])
{
    
    return 0;
}
