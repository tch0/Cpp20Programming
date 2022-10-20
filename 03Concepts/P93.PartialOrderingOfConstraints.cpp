#include <iostream>
#include <concepts>
#include <type_traits>

namespace function_overloading
{

template<typename T> requires std::integral<T>
void f(T*)
{
    std::cout << "f(T*)" << std::endl;
}

template<typename T>
void f(const T*)
{
    std::cout << "f(const T*)" << std::endl;
}

template<typename T> requires std::integral<T>
void g(T*)
{
    std::cout << "g(T*): constrained version" << std::endl;
}

template<typename T>
void g(T*)
{
    std::cout << "g(T*): normal version" << std::endl;
}

template<typename T, typename U> requires std::integral<T> && std::integral<U>
void h(T*, const U*)
{
    std::cout << "h(T*, const U*)" << std::endl;
}

template<typename T, typename U>
void h(const T*, U*)
{
    std::cout << "h(const T*, U*)" << std::endl;
}

} // namespace function_overloading

namespace class_template_partial_specialization
{

template<typename T>
struct A
{
    void foo()
    {
        std::cout << "normal version" << std::endl;
    }
};

template<typename T> requires std::integral<T>
struct A<T>
{
    void foo()
    {
        std::cout << "constrained version" << std::endl;
    }
};


} // namespace class_template_partial_specialization

namespace identical_of_constraints
{

template<typename T> requires std::is_integral_v<T> && std::is_signed_v<T>
void f(T)
{
    std::cout << "f(T): signed integral version" << std::endl;
}
template<typename T> requires std::is_integral_v<T>
void f(T)
{
    std::cout << "f(T): integral version" << std::endl;
}

template<typename T> requires std::integral<T> && std::is_signed_v<T>
void g(T)
{
    std::cout << "g(T): signed integral version" << std::endl;
}
template<typename T> requires std::integral<T>
void g(T)
{
    std::cout << "g(T): integral version" << std::endl;
}

} // namespace identical_of_constraints


int main(int argc, char const *argv[])
{
    {
        using namespace function_overloading;
        const int a = 0;
        f(&a); // f(const T*)
        g(&a); // g(T*): constrained version
        
    }
    {
        std::cout << std::endl;
        using namespace class_template_partial_specialization;
        A<int> a;
        a.foo();
    }
    {
        std::cout << std::endl;
        using namespace identical_of_constraints;
        f(1u);
        // f(1); // ambiguous
        g(1u);
        g(1);
    }
    
    return 0;
}
