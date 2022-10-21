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

// range utilities:
// view_interface: create own cheap view, used through CRTP

// subrange: modeling a range, implement view_interface
// ref_view: reference of a range, implement view_interface

// range factories:
// view is range with cheap delivery overhead, standard library provide some factories to create view, in namespace std::views
// ranges::empty_view / views::empty : create empty view
auto v1 = ranges::empty_view<int>{};
// equal to
auto v2 = views::empty<int>;
static_assert(ranges::view<decltype(v1)>);
static_assert(v1.empty());
static_assert(ranges::view<decltype(v2)>);
static_assert(v2.empty());

// ranges::single_view / views::single : create view which has one element
auto v3 = ranges::single_view{6};
auto v4 = views::single(6);
static_assert(ranges::view<decltype(v3)>);
static_assert(ranges::size(v3) == 1);
static_assert(ranges::view<decltype(v4)>);
static_assert(ranges::size(v4) == 1);

// ranges::iota_view / views::iota : create a view with increasing numbers
ranges::iota_view v5(0); // infinite range, with lazy evaluation, calculate only when iteration.
auto v6 = views::iota(1, 23); // include left but exclude right
static_assert(ranges::view<decltype(v5)>);
static_assert(ranges::view<decltype(v6)>);

// ranges::istream_view / views::istream : create view from input stream
ranges::istream_view<int> v7(std::cin);
auto v8 = views::istream<int>(std::cin);
static_assert(ranges::view<decltype(v7)>);
static_assert(ranges::view<decltype(v8)>);

int main(int argc, char const *argv[])
{
    return 0;
}
