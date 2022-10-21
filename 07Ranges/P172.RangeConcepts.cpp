#include <iostream>
#include <ranges>
#include <vector>
#include <array>
#include <list>
#include <string>
#include <stack>
#include <string_view>
#include <span>
#include <map>
#include <forward_list>
#include <unordered_map>

namespace ranges = std::ranges;
namespace views = std::views;

// concepts in std::ranges:
//      see https://zh.cppreference.com/w/cpp/header/ranges
// std::ranges::range: types that allow iterate it's elements, requires can return it's begin and end sentinel
static_assert(ranges::range<std::vector<int>>);
static_assert(ranges::range<std::list<int>>);
static_assert(ranges::range<char[10]>);
static_assert(!ranges::range<std::stack<int>>);

// std::ranges::borrowed_range : a borrowed range, reference to other range, do not hold the value, just refer to it, should avoid dangling (lvalue) reference. 
//      std::ranges::enable_borrowed_range: is a bool template variable, specilize for your borrowed range to true, default is false.
static_assert(ranges::borrowed_range<std::string_view>);
static_assert(!ranges::borrowed_range<std::vector<int>>);

// std::ranges::sized_range: ranges::range && can get size
//      associated variable template: std::ranges::disable_sized_range
static_assert(ranges::sized_range<std::vector<int>>); // all standard container fulfill this concept

// std::ranges::view : ranges::range && can be move/move assign and destruct at constant time, if support copy, also need to be done at constant time.
//      all are semantic constraints, so we need a variable template for user to specialze.
//      associated variable template: std::ranges::enable_view
// all standard library containers are not view
// string_view is view
static_assert(!ranges::view<std::vector<int>>);
static_assert(ranges::view<std::string_view>);
static_assert(ranges::view<std::span<int>>);

// other concepts:
//      iterators:  input_range, output_range, forward_range, bidirectional_range, random_access_range, contiguous_range, common_range, viewable_range.
static_assert(ranges::bidirectional_range<std::map<int, std::string>>);
static_assert(ranges::forward_range<std::unordered_map<int, double>>);
static_assert(ranges::random_access_range<std::string>);
static_assert(ranges::contiguous_range<std::string>);

// std::ranges::common_range : begin and end are same types, all standard library containers are common range
static_assert(ranges::common_range<std::forward_list<int>>);
static_assert(ranges::common_range<int[10]>);

// std::ranges::viewable_range : can be treated as view safely, view and referenced range are viewable range
static_assert(ranges::viewable_range<std::string_view>);

int main(int argc, char const *argv[])
{
    return 0;
}
