#include <iostream>
#include <cmath>
#include <type_traits>

constexpr double sum(double a, double b)
{
    if (std::is_constant_evaluated())
    {
        return a + b;
    }
    else
    {
        std::cout << "runtime version" << std::endl;
        return a + b;
    }
}

static_assert(std::is_constant_evaluated()); // always true in compile time expressions
int y = 0;
const int a = std::is_constant_evaluated() ? y : 1; // 1, std::is_constant_evaluated() return true here
const int b = std::is_constant_evaluated() ? 2 : y; // 2, std::is_constant_evaluated() return false here

int main(int argc, char const *argv[])
{
    constexpr double res1 = sum(10, 2); // compile-time version
    std::cout << res1 << std::endl;
    double res2 = sum(10, 2); // runtime version
    std::cout << res2 << std::endl;

    std::cout << a << " " << b << std::endl;
    return 0;
}
