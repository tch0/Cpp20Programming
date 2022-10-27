#include <iostream>
#include <concepts>
#include <utility>
#include <type_traits>

// An invasive static reflection implementation

// use macro to calculate, like GET_ARG_COUNT(a, b, c) will produce 3
#define GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, n, ...) n
#define GET_ARG_COUNT(...) GET_NTH_ARG(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

// macros that do repeat a meta macro for sepcific times
#define REPEAT_0(f, i, arg)
#define REPEAT_1(f, i, arg)      f(i, arg)
#define REPEAT_2(f, i, arg, ...) f(i, arg) REPEAT_1(f, i+1, __VA_ARGS__)
#define REPEAT_3(f, i, arg, ...) f(i, arg) REPEAT_2(f, i+2, __VA_ARGS__)
#define REPEAT_4(f, i, arg, ...) f(i, arg) REPEAT_3(f, i+3, __VA_ARGS__)
#define REPEAT_5(f, i, arg, ...) f(i, arg) REPEAT_4(f, i+4, __VA_ARGS__)
#define REPEAT_6(f, i, arg, ...) f(i, arg) REPEAT_5(f, i+5, __VA_ARGS__)
#define REPEAT_7(f, i, arg, ...) f(i, arg) REPEAT_6(f, i+6, __VA_ARGS__)
#define REPEAT_8(f, i, arg, ...) f(i, arg) REPEAT_7(f, i+7, __VA_ARGS__)

// due to the expression ability of macros, we need put the type in parentheses
#define PARE(...) __VA_ARGS__
#define PAIR(x) PARE x // PAIR((int) x) => PAIR(int) x => int x
#define EAT(...)
#define STRIP(x) EAT x // STRIP((int) x) => EAT(int) x => x
// concat two symbols
#define CONCATE(x, y) x ## y
#define PASTE(x, y) CONCATE(x, y)

// convert symbol to C-style string
#define STRING(x) #x

#define FIELD_EACH(i, arg)                                                      \
    PAIR(arg);                                                                  \
    template<typename T>                                                        \
    struct FIELD<T, i> {                                                        \
        T& obj;                                                                 \
        auto value() -> decltype(auto) { return obj.STRIP(arg); }               \
        static constexpr const char* name() { return STRING(STRIP(arg)); }      \
    };                                                                          \

#define DEFINE_SCHEMA(st, ...)                                                  \
    struct st {                                                                 \
        template<typename, size_t> struct FIELD;                                \
        static constexpr size_t _field_count = GET_ARG_COUNT(__VA_ARGS__);      \
        PASTE(REPEAT_, GET_ARG_COUNT(__VA_ARGS__)) (FIELD_EACH, 0, __VA_ARGS__) \
    }                                                                           \

// check whther a type has refelective meta data.
template<typename T>
concept Reflected = requires(T obj)
{
    { obj._field_count } -> std::convertible_to<size_t>;
    requires (obj._field_count == 0) ||  // no data member
        (obj._field_count > 0 && requires {
            typename std::decay_t<T>::template FIELD<T, 0>; // or has data member and has nested type FIELD
        });
};

// example
DEFINE_SCHEMA(Point,
    (double) x,
    (double) y);

static_assert(Reflected<Point>);

// see https://github.com/netcan/config-loader for more details

int main(int argc, char const *argv[])
{
    return 0;
}
