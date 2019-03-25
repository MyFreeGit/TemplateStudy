#include <memory>

/*  Now we can wrap all kind of callable object to Functor.
    The short comming for this implementation:
        When we try to create a Functor object with a non callable object such as:
            Functor<void(void)> f = 12;
        the compiling error isn't very clear.
    We can implement a Type traits for a more clear output.
*/
template<typename R, typename... Args>
class BaseBridge
{
public:
    virtual R invoke(Args... args) = 0;
};

template<typename Callable, typename R, typename... Args>
class SpecificBridge : public BaseBridge<R, Args...>
{
public:
    SpecificBridge(Callable f) : func(f) {}
    virtual R invoke(Args... args) override
    {
        return func(std::forward<Args>(args)...);
    }
private:
    Callable func;
};

// Primary Generic default template
template<typename F>
class Functor;

template<typename R, typename... Args>
class Functor<R(Args...)>
{
public:
    template<typename F>
    Functor(F func) :
        // Using decay_t to remove the RVal reference in bridge type (the wrapped callable object shall not be ref).
        bridge(new SpecificBridge<std::decay_t<F>, R, Args...>(std::forward<F>(func))) {}

    Functor(Functor&& functor)
    {
        bridge = std::move(functor.bridge);
    }

    void operator = (const Functor &functor)
    {
        this->bridge = functor.bridge;
    }

    R operator () (Args... args)
    {
        return bridge->invoke(std::forward<Args>(args)...);
    }
private:
    std::shared_ptr<BaseBridge<R, Args...>> bridge;
};