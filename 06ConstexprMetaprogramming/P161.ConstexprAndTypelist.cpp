#include <iostream>
#include <type_traits>
#include <concepts>
#include <variant>

// constexpr style type calculation
// key point: the interchange of type and value

// typelist
template<typename... Ts>
struct TypeList
{
    struct IsTypeList {};
    using type = TypeList;
    consteval size_t size() const { return sizeof...(Ts); }
    template<typename... T> using append = TypeList<Ts..., T...>;
    template<typename... T> using prepend = TypeList<T..., Ts...>;
    template<template<typename...> class T> using To = T<Ts...>;
};

template<typename... LHS, typename... RHS>
consteval bool operator==(TypeList<LHS...>, TypeList<RHS...>)
{
    if constexpr (sizeof...(LHS) == sizeof...(RHS))
    {
        return (... && std::same_as<LHS, RHS>);
    }
    else
    {
        return false;
    }
}

// convert TypeList to value, so that it can be passed to constexpr function
template<typename T>
struct TypeConst { using type = T; }; // same as std::type_identity
template<typename T>
inline constexpr TypeConst<T> _t;

template<typename... Ts>
inline constexpr auto type_list = TypeList<TypeConst<Ts>...>{};

// high order functions (implement as lambdas)
inline constexpr auto transform_impl = []<typename F, typename... Ts>(TypeList<Ts...>, F) -> TypeList<std::invoke_result_t<F, Ts>...> { return {}; };

namespace test_transform_impl
{

auto tl = type_list<int, char, double>;
auto res = transform_impl(tl, []<typename T>(TypeConst<T>) {
    return _t<std::add_pointer_t<T>>;
});
static_assert(res == type_list<int*, char*, double*>);

} // namespace test_transform_impl

// filter: constexpr implementation
// inline constexpr auto filter = []<typename P, typename... Ts>(TypeList<Ts...>, P) -> detail::Filter_t<P, Ts...> { return {}; };


// see https://github.com/netcan/meta-list for more details (like details::Filter_t)

int main(int argc, char const *argv[])
{
    
    return 0;
}
