#include <iostream>
#include <memory>

class Point
{
private:
    double x;
    double y;
public:
    Point(double _x = 0.0, double _y = 0.0) : x(_x), y(_y)
    {
        std::cout << "Point(): " << x << ", " << y << std::endl;
    }
    ~Point()
    {
        std::cout << "~Point(): " << x << ", " << y << std::endl;
    }
};

// Object::submit should only called by object that managed by std::shared_ptr
// so we usually should avoid create it from stack (could be done by define constructors as private and provide factory to create objects)
struct Object : public std::enable_shared_from_this<Object>
{
public:
    // the occasion of registering asynchronous callbacks, just for example
    template<typename Executor>
    void submit(Executor& executor)
    {
        executor.submit([wp = weak_from_this()]() { // weak callback: just capture weak pointer
            if (auto sp = wp.lock()) // if the object still exist, promote to shared_ptr and do callback, else do nothing!
            {
                sp->onCallback();
            }
        });
    }
    void onCallback() {}
public:
    // factory for creating new object
    static std::shared_ptr<Object> create()
    {
        return std::shared_ptr<Object>(new Object()); // can not use make_shared cuz constructor is private.
    }
private:
    Object() = default; // private constructor to avoid create from stack
};

int main(int argc, char const *argv[])
{
    {
        // same behavior
        std::weak_ptr<Point> wp;
        {
            std::shared_ptr<Point> sp(new Point(1.0, 2.0));
            wp = sp;
        }
        std::cout << "shared_ptr destruct" << std::endl;
        // the memory of Point may already be realeased here!
    }
    {
        std::weak_ptr<Point> wp;
        {
            std::shared_ptr<Point> sp = std::make_shared<Point>(1.1, 2.2);
            wp = sp;
        }
        std::cout << "shared_ptr destruct" << std::endl;
        // the memory of Point may not be released yet!
    }
    {
        // typical usage
        auto sp = Object::create();
        auto sp2 = sp->shared_from_this();
        std::cout << sp.use_count() << std::endl;
    }
    return 0;
}
