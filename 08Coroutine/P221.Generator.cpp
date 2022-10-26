#include <iostream>
#include <type_traits>
#include <coroutine>
#include <utility>
#include <exception>
#include <tuple>
#include <concepts>

// a template of Generator
template<typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    struct promise_type
    {
        promise_type() {}
        Generator get_return_object()
        {
            return {handle_type::from_promise(*this)}; // construct Generator through coroutine_handle
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() { }
        template<std::assignable_from<T> From> // I think std::assignable_from is better than std::convertible_to here
        std::suspend_always yield_value(From&& from)
        {
            current_value_ = std::forward<From>(from);
            return {};
        }
        T current_value_;
    };
    T operator()()
    {
        coro_handle_.resume();
        return coro_handle_.promise().current_value_;
    }
    explicit operator bool()
    {
        return !coro_handle_.done();
    }
    Generator(Generator&& rhs) noexcept : coro_handle_(std::exchange(coro_handle_, {})) {}
    ~Generator() // used as RAII class
    {
        if (coro_handle_)
        {
            coro_handle_.destroy();
        }
    }
private:
    Generator(handle_type _h) : coro_handle_(_h) {}
    handle_type coro_handle_;
};

Generator<int> fibo(int n)
{
    int a = 0, b = 1;
    co_yield a;
    while (a < n)
    {
        std::tie(a, b) = std::make_tuple(b, a + b);
        co_yield a;
    }
    co_return;
}

int main(int argc, char const *argv[])
{
    for (auto g = fibo(1000000); g;)
    {
        std::cout << g() << " ";
    }
    return 0;
}
