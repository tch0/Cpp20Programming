#include <iostream>
#include <concepts>
#include <type_traits>

struct Foo { using type = float; };
struct Bar { using type = int; };
struct Buz {};

namespace atomic
{
template<typename T, typename U>
concept C1 = std::is_integral_v<typename T::type> || std::is_integral_v<typename U::type>;

static_assert(C1<Bar, Buz>);

template<typename T, typename U>
concept C2 = bool(std::is_integral_v<typename T::type> || std::is_integral_v<typename U::type>);

static_assert(!C2<Bar, Buz>);
static_assert(C2<Foo, Bar>);

}

namespace negation
{

template<typename T>
concept C1 = std::is_integral_v<typename T::type>; // has nested ::type and is integral

template<typename T>
concept C2 = !std::is_integral_v<typename T::type>; // has nested ::type but is not integral

template<typename T>
concept C3 = !C1<T>; // C3 is negation of C1, do not has ::type or has ::type but is not integral

// struct Foo { using type = float; };

static_assert(C1<int> == false);
static_assert(C2<int> == false);
static_assert(C3<int> == true);
static_assert(C1<Foo> == false);
static_assert(C2<Foo> == true);
static_assert(C3<Foo> == true);

} // namespace negation

namespace variadic
{

template<typename... Ts>
concept C1 = (std::is_integral_v<typename Ts::type> || ...);

static_assert(C1<Foo, Bar>);
static_assert(!C1<Bar, Buz>);

template<typename T>
concept NestedIntegralType = std::is_integral_v<typename T::type>;

template<typename... Ts>
concept C2 = (NestedIntegralType<Ts> || ...);

static_assert(C2<Foo, Bar>);
static_assert(C2<Bar, Buz>);


} // namespace variadic



int main(int argc, char const *argv[])
{



    return 0;
}