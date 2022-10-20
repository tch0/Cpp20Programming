#include <iostream>
#include <tuple>

// CRTP solution for comparison operators
template<typename Derived>
class Comparable
{
public:
    friend bool operator==(const Derived& lhs, const Derived& rhs)
    {
        return lhs.tie() == rhs.tie();
    }
    friend bool operator!=(const Derived& lhs, const Derived& rhs)
    {
        return lhs.tie() != rhs.tie();
    }
    friend bool operator<(const Derived& lhs, const Derived& rhs)
    {
        return lhs.tie() < rhs.tie();
    }
    friend bool operator<=(const Derived& lhs, const Derived& rhs)
    {
        return lhs.tie() <= rhs.tie();
    }
    friend bool operator>(const Derived& lhs, const Derived& rhs)
    {
        return lhs.tie() > rhs.tie();
    }
    friend bool operator>=(const Derived& lhs, const Derived& rhs)
    {
        return lhs.tie() >= rhs.tie();
    }
};

class Point : public Comparable<Point>
{
    double x, y;
public:
    Point(double _x = 0.0, double _y = 0.0) : x(_x), y(_y) {}
    auto tie() const
    {
        return std::tie(x, y);
    }
};

// in C++20, you just need define the spaceship operator
class Point2
{
public:
    double x, y;
    // will compare all memebers and base class subobjects recursively in depth first order
    // return std::strong_ordering by default (but here is partial_ordering cuz the type double, do not understand why yet? just use auto!)
    friend auto operator<=>(const Point2& lhs, const Point2& rhs) = default;
};

int main(int argc, char const *argv[])
{
    Point x{1.0, 2.0}, y{1.1, 2.2};
    std::cout << std::boolalpha;
    std::cout << (x < y) << std::endl;

    Point2 x2{1.0, 2.0}, y2{1.1, 2.2};
    std::cout << (x2 < y2) << std::endl;
    return 0;
}

