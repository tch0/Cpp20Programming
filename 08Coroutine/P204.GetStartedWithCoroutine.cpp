#include <iostream>
#include <type_traits>
#include <coroutine>
#include <tuple>

// the best document of C++ coroutine: https://en.cppreference.com/w/cpp/language/coroutines

// just for representation
// Generator is not a valid type yet, so it won't pass the complition
Generator fibo()
{
    int a = 0, b = 1;
    while (a < 1000000)
    {
        co_yield a; // use co_yield to give up control, a and b will be preserved
        std::tie(a, b) = std::make_tuple(b, a + b);
    }
    co_return;
}

int main(int argc, char const *argv[])
{
    for (auto f = fibo(); !f.done(); f.next())
    {
        std::cout << f.current_value() << std::endl;
    }
    return 0;
}
