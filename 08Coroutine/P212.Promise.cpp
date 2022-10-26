#include <iostream>
#include <type_traits>
#include <concepts>
#include <coroutine>
#include <string>
#include <utility>

// future and promise: 
//      promise support value in coroutine
//      future get value in where call the coroutine

// C++ standard do not emphasize the concept of future, but we called the return type of coroutine as future.
// future has a associated promise type, and the promise object is saved in coroutine_handle.

// frame of coroutine: 
//      resume_fn(frame_ptr*)
//      destroy_fn(frame_ptr*)
//      promise_type
//      arguments of coroutine
//      states of coroutine (like points of suspending)
//      local variables of coroutine

// the creation of coroutine:
//      allocate coroutine frame
//      passing arguments to coroutine
//      construct promise object
//      use promise object construct future object, and return future object to it's caller

// the association between future and promise:
//      Future::promise_type
//      std::coroutine_traits<Future>::promise_type (equal to Future::promise_type, can be specialized for built-in types and other types that can not be modififed)

// if arguments of coroutine match the constructor of promise:
//      corresponding constructor of promise will be called with the arguments passing to the coroutine.
//      when member function or lambda are used as coroutine, the first argument is the object(*this) itself.
//      consider this point when implement promise.

struct Future
{
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    int v;
    Future(int _v) : v(_v) {}
    Future(Future&& rhs) noexcept : coro_handle_(std::exchange(rhs.coro_handle_, {})) {}
    struct promise_type
    {
        int val;
        template<typename Obj>
        promise_type(Obj&&, int v) : val(v)
        {
            std::cout << "member or lambda coroutine, v = " << v << std::endl;
        }
        promise_type(int v) : val(v)
        {
            std::cout << "free coroutine, v = " << v << std::endl;
        }
        promise_type(std::string s) : val(-100)
        {
            std::cout << "free coroutine with std::string argument, str = " << s << std::endl;
        }
        promise_type() : val(0)
        {
            std::cout << "default constructor" << std::endl;
        }
        // all intefaces that should be implemented

        // the Future returned, return to caller the first time suspend. (if coroutine suspend at initial_suspend, then initial_suspend is the first time)
        Future get_return_object()
        {
            return {handle::from_promise(*this)};
        }
        // before do anything in coroutine, will do a initial suspend first. we do nothing here, so return std::suspend_never(shouble return an Awaitable).
        // do not throw exception here.
        std::suspend_never initial_suspend() noexcept
        {
            std::cout << "initial suspend" << std::endl;
            return {};
        }
        // yield_value: customize behaviour of co_yield
        // co_yield is just a syntactic sugar of co_await: (co_yield a) will become co_await promise.yield_value(a)
        int current_value_;
        auto yield_value(int value)
        {
            current_value_ = value;
            return std::suspend_always{};
        }
        // await_transform: customize co_await, construct Awaitable from expression of co_await.
        // ignore await_transform here

        // return_value and return_void: customize co_return
        // co_return expr; and expr is not void, the expr become argument of return_value
        // int return_value(int)
        // {
        //     return val;
        // }
        // 1. co_return;
        // 2. co_return expr; and expr is void
        void return_void()
        {
            std::cout << "return void, done: " << std::boolalpha << handle::from_promise(*this).done() << std::endl;
            return;
        }
        // return_value and return void should only declare one of them.

        // before final_suspend, all local variables(but not arguments) will be destructed.

        // all exceptions will be handled by unhandled_exception
        // usually call std::current_exception() get a copy exception for rethrowing when users fetch value from Future
        void unhandled_exception()
        {
            // do nothing yet
        }
        // final_suspend
        // after return will call a final co_await promie.find_suspend()
        // the coroutine will go to here no matter throw an excpetion or co_return.
        // if user do not suspend at this point, the coroutine will be freed at this point
        // if user suspend, then the coroutine will exist until Future destroy.
        // usually suspend (by return std::suspend_always)
        std::suspend_always final_suspend() noexcept
        {
            std::cout << "final suspend, done: " << std::boolalpha << handle::from_promise(*this).done() << std::endl;
            return {};
        }
    };
private:
    Future(handle h) : coro_handle_(h) {}
    handle coro_handle_; // save coroutine handle, used as RAII class, the coroutine handle will be released when Future destruct.
};

// the destruction procedure of coroutine:
//      destruct the promise object
//      destruct arguments of coroutine
//      release coroutine frame
//      return to caller/recoverer of coroutine

struct Test
{
    Future member_coro(int) { co_return; }
};

Future free_coro(int)
{
    co_return;
}

Future free_coro2(std::string)
{
    co_return;
}

auto lambda_coro = [](int) -> Future { co_return; };

int main(int argc, char const *argv[])
{
    Test obj;
    obj.member_coro(1);
    lambda_coro(2);
    free_coro(3);
    free_coro2("hello");
    return 0;
}
