#include <iostream>

int main(int argc, char const *argv[])
{
    constexpr auto add = []<typename T>(auto b) { T a{}; return a + b; };
    std::cout << add.operator()<int>(2.0) << std::endl;
    return 0;
}
