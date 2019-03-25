#include <memory>

/* The functor implementation demonstrate the most basic Functor class only can take
   two kind of callable object:
      - Functor for R(Args...) kind of function.
      - An Function Pointer to R(Args...)
    The improvement to Step 1 is, now we can wrapper function and functor with any kind of return type
    and variadic parameter.
    The gap to the target is:
      - The Functor object can only be the specialization of template UniversalFunctor
      - Still cannot assign the labdam function to Functor object
*/
// Generic Template
template<typename F>
struct UniversalFunctor;

// Template specialization for template argument is a function type.
template<typename R, typename... Args>
struct UniversalFunctor<R(Args...)>
{
    R operator() (Args...);
};

// Define a template function pointer. Note: the typedef doesn't supoort template parameter
template <typename R, typename... Args>
using FuncPtr = R(*)(Args...);

// Templatelized BaseFuncBridge
template<typename R, typename... Args>
class BaseFuncBridge
{
public:
    virtual R invoke(Args...) = 0;
};

// Store the specialized UniversalFunctor
template<typename R, typename... Args>
class FunctorBridge : public BaseFuncBridge<R, Args...>
{
public:
    FunctorBridge(const UniversalFunctor<R(Args...)>& functor) : func(functor) {}
    virtual R invoke(Args... args) override
    {
        return func(std::forward<Args>(args)...);
    }
private:
    UniversalFunctor<R(Args...)> func;
};

// Store the templated function pointer
template<typename R, typename... Args>
class FuncPtrBridge : public BaseFuncBridge<R, Args...>
{
public:
    FuncPtrBridge(const FuncPtr<R, Args...>& funcPtr) : func(funcPtr) {}
    virtual R invoke(Args... args) override
    {
        return func(std::forward<Args>(args)...);
    }
private:
    FuncPtr<R, Args...> func;
};

// Generic template
template<typename F>
class Functor;

// Special Template for function type
template<typename R, typename... Args>
class Functor<R(Args...)>
{
public:
    Functor(const FuncPtr<R, Args...> funcPtr)
        : bridge(new FuncPtrBridge<R, Args...>(funcPtr)) {}

    Functor(const UniversalFunctor<R(Args...)>& functor)
     : bridge(new FunctorBridge<R, Args...>(functor)) {}

    Functor(Functor&& functor)
    {
        bridge = std::move(functor.bridge);
    }

    void operator = (const Functor &functor)
    {
        this->bridge = functor.bridge;
    }

    // Now we need to use the perfect forward to forward call to wrapped callable object.
    R operator () (Args... args)
    {
        return bridge->invoke(std::forward<Args>(args)...);
    }
private:
    std::shared_ptr<BaseFuncBridge<R, Args...>> bridge;
};