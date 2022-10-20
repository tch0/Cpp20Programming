#include <iostream>
#include <utility>
#include <type_traits>
#include <concepts>
#include <tuple>
#include <variant>

// 5.3 TypeList

template<typename... Ts>
struct TypeList
{
    struct IsTypeList {}; // mark that this a TypeList, used in concept
    using type = TypeList;
    static constexpr size_t size = sizeof...(Ts);
    template<typename... T> using append = TypeList<Ts..., T...>;
    template<typename... T> using prepend = TypeList<T..., Ts...>;
    // nested meta function, convert this TypeList to other class template instance
    template<template<typename...> typename T> using To = T<Ts...>;
};

template<typename TypeList>
concept TL = requires {
    typename TypeList::IsTypeList;
    typename TypeList::type;
};

// test
namespace test_typelist
{

using AList = TypeList<int, char>;
static_assert(TL<AList>);
static_assert(AList::size == 2);
static_assert(std::same_as<AList::prepend<double>, TypeList<double, int, char>>);
static_assert(std::same_as<AList::append<double>, TypeList<int, char, double>>);
static_assert(std::same_as<AList::To<std::tuple>, std::tuple<int, char>>);
static_assert(std::same_as<AList::To<std::variant>, std::variant<int, char>>);

} // namespace test_typelist


// high-order functions

// map
template<TL In, template<typename> class F> // accept a TypeList and an unary meta function
struct Map;
template<template<typename> class F, typename... Ts>
struct Map<TypeList<Ts...>, F> : TypeList<typename F<Ts>::type...> {};

template<TL In, template<typename> class F>
using Map_t = typename Map<In, F>::type;

// filter
template<TL In, template<typename> class P, TL Out = TypeList<>> // accept a TypeList and an unary meta predicate
struct Filter : public Out {}; // end of recursion
template<template<typename> class P, TL Out, typename H, typename... Ts>
struct Filter<TypeList<H, Ts...>, P, Out> 
    : std::conditional_t<P<H>::value, 
        Filter<TypeList<Ts...>, P, typename Out::template append<H>>,
        Filter<TypeList<Ts...>, P, Out>> {};

template<TL In, template<typename> class P>
using Filter_t = typename Filter<In, P>::type;

// fold (left)
template<TL In, typename Init, template<typename, typename> class Op> // Op is a binary meta function
struct Fold : std::type_identity<Init> {}; // end of recursion
template<typename Acc, template<typename, typename> class Op, typename H, typename... Ts>
struct Fold<TypeList<H, Ts...>, Acc, Op> : Fold<TypeList<Ts...>, typename Op<Acc, H>::type, Op> {};

template<TL In, typename Init, template<typename, typename> class Op>
using Fold_t = typename Fold<In, Init, Op>::type;

// test
namespace test_high_order_functions
{

// map
using LongList = TypeList<char, float, double, short, char, unsigned>;
static_assert(std::same_as<Map_t<LongList, std::add_pointer>, TypeList<char*, float*, double*, short*, char*, unsigned*>>);
// filter
template<typename T> using SizeLessThan4 = std::bool_constant<(sizeof(T) < 4)>;
static_assert(std::same_as<Filter_t<LongList, SizeLessThan4>, TypeList<char, short, char>>);
// fold
template<typename Acc, typename T> using TypeSizeAcc = std::integral_constant<size_t, Acc::value + sizeof(T)>;
static_assert(Fold_t<LongList, std::integral_constant<size_t, 0>, TypeSizeAcc>::value == 20); // 1+4+8+2+1+4 (64 bytes environment)

} // namespace test_high_order_functions

// common algorithms

// concat
template<TL... In> struct Concat;
template<TL... In> using Concat_t = typename Concat<In...>::type;

template<> struct Concat<TypeList<>> : TypeList<> {};
template<TL In> struct Concat<In> : In {};
template<TL In1, TL... Rest>
struct Concat<In1, Rest...> : Concat_t<In1, Concat_t<Rest...>> {};
template<typename... Ts1, typename... Ts2>
struct Concat<TypeList<Ts1...>, TypeList<Ts2...>> : TypeList<Ts1..., Ts2...> {};

// elem: given a typelist and an element, find whether the element in the list
template<TL In, typename E>
struct Elem
{
private:
    template<typename Acc, typename T>
    using FindE = std::conditional_t<Acc::value, Acc, std::is_same<T, E>>;
    using Found = Fold_t<In, std::false_type, FindE>;
public:
    constexpr static bool value = Found::value;
};
template<TL In, typename E>
static constexpr bool Elem_v = Elem<In, E>::value;

// second implementation: use fold expression
template<typename In, typename E>
struct Elem2 : std::false_type {};
template<typename E, typename... Ts>
struct Elem2<TypeList<Ts...>, E> : std::bool_constant<(false || ... || std::is_same_v<Ts, E>)> {};

template<TL In, typename E>
static constexpr bool Elem2_v = Elem2<In, E>::value;

// unique: remove duplicate elements
template<TL In>
struct Unique
{
private:
    template<TL Acc, typename E>
    using Append = std::conditional_t<Elem_v<Acc, E>, Acc, typename Acc::template append<E>>;
public:
    using type = Fold_t<In, TypeList<>, Append>;
};
template<TL In>
using Unique_t = typename Unique<In>::type;

// partition: partition the typelist to two list: satisfied one and rest
template<TL In, template<typename> class P>
struct Partition
{
private:
    template<typename Arg>
    using NotP = std::bool_constant<!P<Arg>::value>;
public:
    using Satisfied = Filter_t<In, P>;
    using Rest = Filter_t<In, NotP>;
};

// sort: quick sort
template<TL In, template<typename, typename> class Comp>
struct Sort : TypeList<> {}; // end of recursion

template<template<typename, typename> class Comp, typename Pivot, typename... Ts>
struct Sort<TypeList<Pivot, Ts...>, Comp>
{
private:
    template<typename E> using LT = Comp<E, Pivot>; // E < Pivot
    using P = Partition<TypeList<Ts...>, LT>;
    using SmallerSorted = typename Sort<typename P::Satisfied, Comp>::type;
    using BiggerSorted = typename Sort<typename P::Rest, Comp>::type;
public:
    using type = Concat_t<typename SmallerSorted::template append<Pivot>, BiggerSorted>;
};

template<TL In, template<typename, typename> class Comp>
using Sort_t = typename Sort<In, Comp>::type;


namespace test_common_algorithms
{

// Elem
using ListA = TypeList<char, double, int>;
using ListB = TypeList<unsigned, float, char>;
static_assert(std::same_as<Concat_t<ListA, ListB, ListA>, TypeList<char, double, int, unsigned, float, char, char, double, int>>);
static_assert(Elem_v<ListA, int>);
static_assert(!Elem_v<ListA, float>);
static_assert(Elem2_v<ListA, int>);
static_assert(!Elem2_v<ListA, float>);
// Unqiue
using ListC = TypeList<char, char, float, double, char, int, int>;
static_assert(std::same_as<Unique_t<ListC>, TypeList<char, float, double, int>>);
// Partition
template<typename T> using SizeLessThan4 = std::bool_constant<(sizeof(T) < 4)>;
using SplitBySize4 = Partition<ListC, SizeLessThan4>;
static_assert(std::same_as<SplitBySize4::Satisfied, TypeList<char, char, char>>);
static_assert(std::same_as<SplitBySize4::Rest, TypeList<float, double, int, int>>);
// Sort
template<typename T1, typename T2>
using SizeLessThan = std::bool_constant<(sizeof(T1) < sizeof(T2))>;
static_assert(std::same_as<Sort_t<ListC, SizeLessThan>, TypeList<char, char, char, float, int, int, double>>); // stable?

} // namespace test_common_algorithms


int main(int argc, char const *argv[])
{
    return 0;
}
