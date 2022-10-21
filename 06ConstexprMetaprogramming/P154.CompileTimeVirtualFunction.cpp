#include <iostream>
#include <type_traits>
#include <numbers>
#include <array>
#include <vector>
#include <memory>

using namespace std::numbers;

// memory can be allocated at compile-time, so virtual function also can be called at compile-time
// and the memory allocated at compile-time should be deallocated at compile-time, it you did not deallocated it, an error will occur.

struct Shape
{
    virtual constexpr ~Shape() {}
    virtual constexpr double getArea() = 0;
};

struct Circle : Shape
{
    double r;
    constexpr Circle(double _r = 0) : r(_r) {}
    virtual constexpr double getArea() override
    {
        return pi * r * r;
    }
    virtual constexpr ~Circle() {}
};

struct Rectangle : Shape
{
    double w, l;
    constexpr Rectangle(double _w = 0.0, double _l = 0.0) : w(_w), l(_l) {}
    virtual constexpr double getArea() override
    {
        return w * l;
    }
    virtual constexpr ~Rectangle() {}
};

consteval double testCompileTimeVirtualFunction()
{
    std::vector<Shape*> vec{new Circle(10.0), new Rectangle(2.1, 3.0), new Circle(5)};
    double sum = 0.0;
    for (auto p : vec)
    {
        sum += p->getArea();
        delete p;
    }
    return sum;
}

int main(int argc, char const *argv[])
{
    std::cout << testCompileTimeVirtualFunction() << std::endl;
    return 0;
}
