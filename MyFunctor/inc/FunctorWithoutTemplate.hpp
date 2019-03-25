#include <memory>

/* The functor implementation demonstrate the most basic Functor class only can take
   two kind of function:
      - Functor for an function with int(int, int)
      - An Function Pointer to function with int(int, int)
*/
int normalAddFunction(int i, int j)
{
    std::cout << "normalAddFunctor::(" << i << ", " << j << ") = " << i + j << std::endl;
    return i < j;
}

struct AddFunctor
{
    virtual int operator() (int i, int j)
    {
        std::cout << "MyAddFunctor::(" << i << ", " << j << ") = " << i + j << std::endl;
        return i + j;
    }
};

// Also can use alias define the function pointer
// using AddFuncPtr = int(*)(int, int);
typedef int(*AddFuncPtr)(int, int);

// Using a bridge to record the callable object, and invoke the call to callable object
class BaseFuncBridge
{
public:
    virtual int invoke(int, int) = 0;
};

// Used to record the AddFunctor type
class AddFunctorBridge : public BaseFuncBridge
{
public:
    AddFunctorBridge(const AddFunctor& functor) : func(functor) {}
    virtual int invoke(int x, int y) override
    {
        return func(x, y);
    }
private:
    AddFunctor func;
};

// Used to record the function pointer.
class AddFuncPtrBridge : public BaseFuncBridge
{
public:
    AddFuncPtrBridge(const AddFuncPtr& funcPtr) : func(funcPtr) {}
    virtual int invoke(int x, int y) override
    {
        return func(x, y);
    }
private:
    AddFuncPtr func;
};

// The interface class to user. It hide the callable object's type when user use
// its () operator. Functor forwards the call to internal bridge.
// The benefits for bridge pointer here is: user can use one kind of Functor type
// to refer to two kind of callable objects. In template environment, this can
// reduce many code.
class Functor
{
public:
    Functor(const AddFuncPtr funcPtr) : bridge(new AddFuncPtrBridge(funcPtr)) {}

    Functor(const AddFunctor& functor) : bridge(new AddFunctorBridge(functor)) {}

    Functor(Functor&& functor)
    {
        bridge = std::move(functor.bridge);
    }

    void operator = (const Functor &functor)
    {
        this->bridge = functor.bridge;
    }

    int operator () (int x, int y)
    {
        return bridge->invoke(x, y);
    }
private:
    std::shared_ptr<BaseFuncBridge> bridge;
};