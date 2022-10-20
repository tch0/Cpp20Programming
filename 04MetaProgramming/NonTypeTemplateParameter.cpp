#include <iostream>
#include <type_traits>
#include <string>
#include <concepts>

using namespace std::literals;

// C++20: floating point non-type template parameter
template<double Value>
void f()
{
    std::cout << Value << std::endl;
}

// C++20: literal type non-type template paramter
template<auto Value>
void g()
{
    std::cout << Value << std::endl;
}

template<typename T>
struct A
{
    constexpr A(const T& _a) : a(_a) {}
    T a;
    friend std::ostream& operator<<(std::ostream& os, const A& obj)
    {
        return os << "A: " << obj.a;
    }
};

int main(int argc, char const *argv[])
{
    f<1.0>();
    g<A(1.5)>();
    g<A(2)>();
    static int a;
    g<A(&a)>();
    return 0;
}
