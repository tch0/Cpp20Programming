#include <iostream>
#include <cassert>
#include <vector>
#include <concepts>

namespace ExpressionTemplates
{

// binary expression of scalar types
template<typename T, typename U, typename OP>
struct BinaryExpression
{
    BinaryExpression(const T& lhs, const U& rhs, OP op) : lhs(lhs), rhs(rhs), op(op) {}
protected:
    T lhs;
    U rhs;
    OP op;
};

template<typename T>
concept Container = requires(const T& t)
{
    t[0];
    { t.size() } -> std::convertible_to<size_t>;
};

// binary expression of vector
template<typename T, typename U, typename OP> requires Container<T> && Container<U>
struct BinaryContainerExpression : private BinaryExpression<T, U, OP>
{
    using Base = BinaryExpression<T, U, OP>;
    using Base::Base; // inherit construtors of base
    auto operator[](size_t index) const // lazy evaluation interface
    {
        assert(index < size());
        return Base::op(Base::lhs[index], Base::rhs[index]); // or use this->
    }
    size_t size() const
    {
        assert(Base::lhs.size() == Base::rhs.size());
        return Base::lhs.size();
    }
};

// deduction guides
template<typename T, typename U, typename OP>
BinaryContainerExpression(T, U, OP) -> BinaryContainerExpression<T, U, OP>;

template<Container T, Container U>
auto operator+(const T& lhs, const U& rhs)
{
    auto plus = [](auto x, auto y) { return x + y; };
    return BinaryContainerExpression(lhs, rhs, plus);
}

} // namespace ExpressionTemplates

using namespace ExpressionTemplates;

void printContainer(auto&& v) requires Container<std::decay_t<decltype(v)>>
{
    for (size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    {
        std::vector<int> x{1, 2, 3}, y{1, 2, 3};
        int alpha = 4;
        auto add_scaled = [alpha](auto lhs, auto rhs) { 
            return lhs + alpha * rhs;
        };
        printContainer(BinaryContainerExpression(x, y, add_scaled)); // calculate x + 4*y
    }
    {
        std::vector<int> x{1, 2, 3}, y{1, 2, 3}, z{2, 1, 4};
        printContainer(x + y + z);
    }
    return 0;
}
