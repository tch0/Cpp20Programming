#include <iostream>
#include <coroutine>
#include <concepts>

// Awaitable
// standard library do not have a Awaitable concept yet! let's define it.
template<typename P, typename T>
decltype(auto) get_awaitable(P& p, T&& expr)
{
    if constexpr (requires { p.await_transform(std::forward<T>(expr)); })
    {
        return p.await_transform(std::forward<T>(expr)); // the Promise has await_transform function
    }
    else
    {
        return std::forward<T>(expr); // do not has a await_transform
    }
}

template<typename Awaitable>
decltype(auto) get_awaiter(Awaitable&& awaitable)
{
    if constexpr (requires { awaitable.operator co_await(); }) // member operator co_await
    {
        return std::forward<Awaitable>(awaitable).operator co_await();
    }
    else if constexpr (requires { operator co_await(awaitable); }) // global operator co_await
    {
        return operator co_await(std::forward<Awaitable>(awaitable));
    }
    else
    {
        return std::forward<Awaitable>(awaitable); // return itself
    }
}

// technically, we should call get_awaitable first, but we know nothing about current Promise(it provides await_transform function).
template<typename Expr>
using GetAwaiter_t = decltype(get_awaiter(std::declval<Expr>()));

template<typename A>
concept Awaitable = requires(GetAwaiter_t<A> awaiter, std::coroutine_handle<> handle)
{
    { awaiter.await_ready() } -> std::convertible_to<bool>;
    awaiter.await_suspend(handle);
    awaiter.await_resume();
};

static_assert(Awaitable<std::suspend_always>);
static_assert(Awaitable<std::suspend_never>);

int main(int argc, char const *argv[])
{
    return 0;
}
