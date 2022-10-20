#include <iostream>
#include <type_traits>
#include <concepts>

struct Foo { using type = float; };
struct Bar { using type = int; };
struct Buz {};

namespace requires_clause
{

template<typename T> requires std::is_integral_v<T>
void f(T)
{
    std::cout << "f(T): constrained version" << std::endl;
}

template<typename T>
void f(T)
{
    std::cout << "f(T): normal version" << std::endl;
}

template<typename T> requires std::is_integral_v<typename T::type>
void g(T)
{
    std::cout << "g(T): constrained version" << std::endl;
}

template<typename T>
void g(T)
{
    std::cout << "g(T): normal version" << std::endl;
}

template<typename T> requires (!std::is_integral_v<typename T::type>)
void h(T)
{
    std::cout << "h(T): constrained version" << std::endl;
}

template<typename T>
void h(T)
{
    std::cout << "h(T): normal version" << std::endl;
}

} // namespace requires_clause

namespace conjunction_and_disjunction
{

template<typename T, typename U> requires (std::integral<typename T::type> || std::integral<typename U::type>)
void f(T, U)
{
    std::cout << "f(T, U): constrained version" << std::endl;
}

template<typename T, typename U>
void f(T, U)
{
    std::cout << "f(T, U): normal version" << std::endl;
}

template<typename T, typename U> requires (bool(std::integral<typename T::type> || std::integral<typename U::type>))
void g(T, U)
{
    std::cout << "g(T, U): constrained version" << std::endl;
}

template<typename T, typename U>
void g(T, U)
{
    std::cout << "g(T, U): normal version" << std::endl;
}

} // namespace conjunction_and_disjunction


namespace simplification
{

template<std::integral T, std::integral U>
void f(T, U)
{
    std::cout << "f(T, U)" << std::endl;
}

void g(std::integral auto t, std::integral auto u)
{
    std::cout << "g(T, U)" << std::endl;
}

auto h = [](std::integral auto lhs, std::integral auto rhs) {
    std::cout << "h(T, U)" << std::endl;
};

} // namespace simplification

namespace class_template_partial_specialization
{

template<typename T>
struct A
{
    void foo()
    {
        std::cout << "primary template" << std::endl;
    }
};

template<std::integral T>
struct A<T>
{
    void foo()
    {
        std::cout << "integral version" << std::endl;
    }
};

} // namespace class_template_partial_specialization


namespace class_template_member
{

template<typename T>
struct A
{
    void operator()() requires std::integral<T>
    {
        std::cout << "A::operator(): integral version" << std::endl;
    }
    void operator()() requires (!std::integral<T>)
    {
        std::cout << "A::operator(): non-integral version" << std::endl;
    }
};

// explicit instantiation
template class A<double>;

} // namespace class_template_member


int main(int argc, char const *argv[])
{
    {
        using namespace requires_clause;
        f(1);
        f(1.0);
    }
    {
        std::cout << std::endl;
        using namespace requires_clause;
        g(Foo());
        g(Bar());
        g(Buz());
    }
    {
        std::cout << std::endl;
        using namespace requires_clause;
        h(Foo()); // constrained version
        h(Bar()); // normal version
        h(Buz()); // normal version
    }
    {
        std::cout << std::endl;
        using namespace conjunction_and_disjunction;
        f(Foo(), Bar()); // constrained version
        f(Bar(), Buz()); // constrained version
        f(Foo(), Buz()); // normal version
        g(Foo(), Bar()); // constrained version
        g(Bar(), Buz()); // normal version
        g(Foo(), Buz()); // normal version
    }
    {
        std::cout << std::endl;
        using namespace simplification;
        f(1, 1L);
        g(1, 1L);
        h(1, 1L);
    }
    {
        std::cout << std::endl;
        using namespace class_template_partial_specialization;
        A<int> a;
        a.foo();
        A<double> b;
        b.foo();
    }
    {
        std::cout << std::endl;
        using namespace class_template_member;
        A<int> a;
        a();
        A<double> b;
        b();
    }
    return 0;
}
