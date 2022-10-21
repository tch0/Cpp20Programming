#include <iostream>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <array>

// constexpr function do not force a function can be evaluated at compile-time if it's not used on compile-time expression.
// but consteval will force a function can be evaluated at compile-time or else it will raise an compile error.

consteval int min(std::initializer_list<int> il)
{
    return *std::min(il.begin(), il.end());
}

// memory allocation can be used on compile-time, but the memory it allocated at compile-time should be released at compile-time,
//      can not retain to run time !!!
consteval std::vector<int> sievePrime(int n)
{
    std::vector<bool> marked(n+1, true);
    for (int p = 2; p * p <= n; ++p)
    {
        if (marked[p])
        {
            for (int i = p * p; i <= n; i += p)
            {
                marked[i] = false;
            }
        }
    }
    std::vector<int> result;
    for (int p = 2; p <= n; ++p)
    {
        if (marked[p])
        {
            result.push_back(p);
        }
    }
    return result;
}

consteval size_t primeCount(int n)
{
    return sievePrime(n).size();
}

static_assert(primeCount(10) == 4); // 2, 3, 5, 7

// the only container that can be allocated at compile-time and be retain to runtime is built-in array.
// so consider how to save things in compile-time containers to a built-in array (or std::array) !

// so we need calculate two times at compile-time, first time get the size of result, second time get the result.
// although two times of calculation, it's way less expensive than traditional metaprogramming through template recursive instantiation.
template<int n>
consteval auto savePrimeToArray()
{
    std::array<int, primeCount(n)> result;
    auto primes = sievePrime(n);
    std::copy(primes.begin(), primes.end(), result.data());
    return result;
}

constexpr auto primesLess100 = savePrimeToArray<100>();
static_assert(primesLess100.size() == 25);

int main(int argc, char const *argv[])
{
    std::cout << min({1, 2, 3, -1}) << std::endl;
    for (auto val : primesLess100)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    return 0;
}
