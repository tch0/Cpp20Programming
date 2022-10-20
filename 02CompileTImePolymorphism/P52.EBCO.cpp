#include <iostream>


struct A {}; // empty
struct B
{
    [[no_unique_address]] A a; // C++20 features, no_unique_address attribute, replace EBCO
    int b;
};

int main(int argc, char const *argv[])
{
    std::cout << sizeof(B) << std::endl; // 4
    return 0;
}
