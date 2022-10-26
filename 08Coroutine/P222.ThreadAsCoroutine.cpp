#include <iostream>
#include <thread>
#include <concepts>
#include <coroutine>
#include <future>
#include <exception>
#include <chrono>
#include <numeric>

// extend std::future for using thread as coroutine
// see https://en.cppreference.com/w/cpp/coroutine/coroutine_traits for a typical example.

// an empty tag type
struct AsCoroutine {};
inline constexpr AsCoroutine as_coroutine;

// partially specialize std::coroutine_traits for std::future, must add a tag type, because the C++ standard formulate that the sepecialization of 
//      standard library utility must have a self-deined type to prevent confilict extension for library utilities (like from multiple different 3-rd party libraries).
template<typename T, typename... Args>
struct std::coroutine_traits<std::future<T>, AsCoroutine, Args...>
{
    // Awaiter for std::future<T>
    struct Awaiter
    {
        std::future<T> fut_;
        bool await_ready()
        {
            return fut_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }
        void await_suspend(std::coroutine_handle<> handle)
        {
            std::thread([=, this] {
                fut_.wait();
                handle.resume();
            }).detach();
        }
        decltype(auto) await_resume() { return fut_.get(); }
    };
    struct promise_type : std::promise<T>
    {
        std::future<T> get_return_object() { return this->get_future(); }
        auto initial_suspend() noexcept { return std::suspend_never(); }
        auto final_suspend() noexcept { return std::suspend_never(); } // do not use std::future as an RAII class, this coroutine will be destroyed after final suspend automatically.
        void unhandled_exception() { this->set_exception(std::current_exception()); }
        template<typename U>
        void return_value(U&& value)
        {
            this->set_value(std::forward<U>(value));
        }
        // transform std::future to awaiter
        Awaiter await_transform(std::future<T> fut)
        {
            return { std::move(fut) };
        }
    };
};

// asynchronous summation
// must add a tag paramter to specify it uses thread as coroutine (for the library utilities specilization issue).
template<std::random_access_iterator RandomIt>
std::future<int> parallel_sum(AsCoroutine, RandomIt beg, RandomIt end)
{
    auto len = end - beg;
    if (len == 0)
    {
        co_return 0;
    }
    RandomIt mid = beg + len / 2;
    auto rest_task = std::async([](RandomIt b, RandomIt e) {
            return std::accumulate(b, e, 0);
        }, mid, end);
    auto first_task = parallel_sum(as_coroutine, beg, mid);
    auto first = co_await std::move(first_task);
    auto rest = co_await std::move(rest_task);
    co_return first + rest;
}

int main(int argc, char const *argv[])
{
    int A[] = {1, 2, 3, 4, 5};
    std::cout << parallel_sum(as_coroutine, std::begin(A), std::end(A)).get() << std::endl; // 15
    return 0;
}
