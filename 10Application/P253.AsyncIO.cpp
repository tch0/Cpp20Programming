#include <iostream>
#include <concepts>
#include <type_traits>
#include <coroutine>
#include <string>
#include <sstream>
#include <source_location>

// see https://github.com/netcan/asyncio for more details

// everything that want to be scheduled should implement Handle and register with the scheduler
// everything can be scheduled, coroutine, normal function, the scheduler and coroutine are decoupled
using HandleId = uint64_t;
struct Handle
{
    enum State : uint8_t
    {
        UNSCHEDULED,
        SUSPEND,
        SCHEDULED
    };
    Handle() noexcept : handle_id_(handle_id_generation_++) {}
    virtual void run() = 0;
    void set_state(State state) { state_ = state; }
    HandleId get_handle_id() { return handle_id_; }
    virtual ~Handle() = default;
private:
    HandleId handle_id_;
    static HandleId handle_id_generation_;
protected:
    State state_ = Handle::UNSCHEDULED;
};

// coroutine handle
struct CoroutineHandle : Handle
{
    std::string frame_name() const
    {
        const auto& frame_info = get_frame_info();
        std::ostringstream os;
        os << frame_info.function_name() << " at " << frame_info.file_name() << ":" << frame_info.line();
        return os.str();
    }
    virtual void dump_backtrace(size_t depth = 0) const {};
    void schedule();
    void cancel();
private:
    virtual const std::source_location& get_frame_info() const; // in order to record the call chain of coroutine.
};

int main(int argc, char const *argv[])
{
    
    return 0;
}
