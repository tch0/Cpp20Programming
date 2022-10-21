#include <iostream>
#include <ranges>

// header: <ranges>
// namespace std::ranges and std::view
// ranges is a functional range library introduced since C++20.
// we always use global function manipulate ranges rather than member functions.


// namesapce alias
namespace ranges = std::ranges;
namespace views = std::views; // it's alias of std::ranges::views

// visit ranges:
// in namespace std::ranges: begin, end, rbegin, rend, size, swap, empty, data/cdata
//      end, rend, cend, crend will return a setinel object.

// contiguous iterator: more constrained random access iterator, it requries the elements are contiguous in memory.

int main(int argc, char const *argv[])
{
    
    return 0;
}
