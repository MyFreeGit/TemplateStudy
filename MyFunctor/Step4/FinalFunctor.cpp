#include <iostream>
#include "FinalFunctorWithTraits.hpp"
#include <functional>

template<typename F, typename R, typename... Args>
struct IsCallableT
{
    static constexpr bool value = std::is_same_v<R, decltype(std::declval<F>(std::declval<Args>...))>;
};

int normalAddFunction(int x1, int x2, int x3)
{
    std::cout << "normalAddFunctor::(" << x1 << ", " << x2 << ", " << x3 <<") = " << x1 + x2 + x3 << std::endl;
    return x1 + x2 + x3;
}

struct AddFunctor
{
    int operator() (int i, int j)
    {
        std::cout << "MyAddFunctor::(" << i << ", " << j << ") = " << i + j << std::endl;
        return i + j;
    }
};

int main(int, char**) {
    Functor<int(int, int)> f1 = AddFunctor();
    Functor<int(int, int, int)> f2 = normalAddFunction;
    int start = 5;
    Functor<void(int, int)> f3 = [start](int i, int j)
                {
                    std::cout << "lambda::(" << i << ", " << j << ") with start(" << start << ") = " << i + j + start << std::endl;
                };
    f1(3,4);
    f2(5, 6, 7);
    f3(1, 2);
    Functor<void(void)> should_fail = 15;
}