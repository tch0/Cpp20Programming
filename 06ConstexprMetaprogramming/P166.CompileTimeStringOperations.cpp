#include <iostream>
#include <concepts>
#include <type_traits>
#include <array>
#include <algorithm>

// final goal: 
//      concat compile-time strings
//      more generic version: join, concat is just a special version of join

// the compile time string may saved on lvalue (like C-style string literal), consider trailing '\0'
// so we should get the rusult type like std::array<char, N+M+1> from maybe const char[] and std::array<char>

// get length of string
template<typename T>
constexpr auto strLength = strLength<std::remove_cvref_t<T>>;

template<size_t N>
constexpr size_t strLength<char[N]> = N-1;
template<size_t N>
constexpr size_t strLength<std::array<char, N>> = N-1;

namespace test_length
{

static_assert(strLength<decltype("hello")> == 5);

} // namespace test_length

// helper of joins
template<typename DelimType, size_t N>
struct JoinStringFold
{
    DelimType delimiter;
    std::array<char, N+1> joinedStr{};
    decltype(joinedStr.begin()) pstr = joinedStr.begin(); // record iterated pointer
    consteval JoinStringFold(DelimType _delimiter) : delimiter(_delimiter) {}
    template<typename STR>
    friend consteval decltype(auto) operator+(JoinStringFold&& self, STR&& str)
    {
        self.pstr = std::copy_n(std::begin(str), strLength<STR>, self.pstr);
        if (self.joinedStr.end() - self.pstr > strLength<DelimType>) // not last stringa
        {
            self.pstr = std::copy_n(self.delimiter, strLength<DelimType>, self.pstr);
        }
        return std::forward<JoinStringFold>(self);
    }
};

// the final size of result of join: Sum(ln) + (N-1)dl, ln is length of string, dl is length of delimiter
template<typename DelimType, typename... STRs>
consteval auto join(DelimType&& delimiter, STRs&&... strs)
{
    constexpr size_t strNum = sizeof...(STRs);
    constexpr size_t len = (0 + ... + strLength<STRs>) + (strNum >= 1 ? strNum - 1 : 0) * strLength<DelimType>;
    return (JoinStringFold<DelimType, len>{std::forward<DelimType>(delimiter)} + ... + std::forward<STRs>(strs)).joinedStr;
}

// implement concat through join
template<typename... STRs>
consteval auto concat(STRs&&... strs)
{
    return join("", std::forward<STRs>(strs)...);
}

namespace test_join
{

constexpr auto one_two = concat("one", "two");
constexpr auto one_two_three = concat(one_two, "three");
static_assert(one_two_three.size() == 12);
static_assert(strLength<decltype(one_two_three)> == 11);
constexpr char res[] = "onetwothree";
static_assert(std::equal(one_two_three.begin(), one_two_three.end(), std::begin(res)));

constexpr auto join_res = join(",", "one", "two", "three");
constexpr char join_expected[] = "one,two,three";
static_assert(std::equal(std::begin(join_res), std::end(join_res), std::begin(join_expected)));

} // namespace test_join


int main(int argc, char const *argv[])
{
    return 0;
}
