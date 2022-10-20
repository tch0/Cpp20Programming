#include <iostream>
#include <concepts>
#include <type_traits>

template<typename T>
concept C1 = std::is_integral_v<typename T::type>; // has nested ::type and is integral

template<typename T>
concept C2 = !std::is_integral_v<typename T::type>; // has nested ::type but is not integral

template<typename T>
concept C3 = !C1<T>; // C3 is negation of C1, do not has ::type or has ::type but is not integral

struct Foo { using type = float; };

int main(int argc, char const *argv[])
{
    static_assert(C1<int> == false);
    static_assert(C2<int> == false);
    static_assert(C3<int> == true);
    static_assert(C1<Foo> == false);
    static_assert(C2<Foo> == true);
    static_assert(C3<Foo> == true);
    return 0;
}
