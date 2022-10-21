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

// range adpator: implement as function obejct, composite with each other with pipe operator (|)
//                the calculation will always defer to the time of iterattion.

// adaptors: in std::ranges
//      all(R): change range to view
//      filter(R, P): filter view
//      transform(R, F): map
//      take(R, N): take first N elements of view
//      take_while(R, P): take elements until P is false
//      drop(R, N): drop first N elements of R
//      drop_while(R, P): drop elements until P is false
//      join(R): R is range of range, flat two dimension range to one dimension range
//      split(R, delimiter): mainly for string process, split view to two dimension view according to delimiter
//      common(R): get common_view of given range which has same begin and end sentinel type (mainly for traditional standard library algorithms).
//      reverse(R): reverse a view
//      elements<N>(R): get range of N-th element of tuple-like object, R is range of tuple, e.g. iterate keys of std::map (see below)

// all of this are easy to use, just like functional language.

int main(int argc, char const *argv[])
{
    std::vector<int> vec{0, 1, 2, 3, 4, 5};
    auto even = [](int i) { return i % 2 == 0; };
    auto square = [](int x) { return x * x; };
    // pipe style
    for (auto v : vec | views::filter(even) | views::transform(square))
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    // function call style
    for (auto v : views::transform(views::filter(vec, even), square))
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    
    // iterate values
    std::map<int, std::string> m{{1, "hello"}, {2, "world"}, {3, "yes"}};
    for (auto s : m | views::elements<1>)
    {
        std::cout << s << " "; // hello world yes
    }
    std::cout << std::endl;
    return 0;
}
