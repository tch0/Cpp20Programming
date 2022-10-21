#include <iostream>
#include <type_traits>

template<typename... Args>
void print(const Args&... args)
{
    (..., (std::cout << args << " ")) << std::endl;
}

int main(int argc, char const *argv[])
{
    print(1, 2, "hello");
    return 0;
}
