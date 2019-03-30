#include <iostream>
#include "FunctorWithVariadicTemplate.hpp"

int normalAddFunction(int x1, int x2, int x3)
{
    std::cout << "normalAddFunctor::(" << x1 << ", " << x2 << ", " << x3 <<") = " << x1 + x2 + x3 << std::endl;
    return x1 + x2 + x3;
}

template<>
struct UniversalFunctor<int(int, int)>
{
    int operator() (int i, int j)
    {
        std::cout << "MyAddFunctor::(" << i << ", " << j << ") = " << i + j << std::endl;
        return i + j;
    }
};

int main(int, char**)
{
    Functor<int(int, int, int)> f1 = normalAddFunction;
    f1(5, 6, 7);
    Functor<int(int, int)> f2 = UniversalFunctor<int(int, int)>();
    f2(3,4);
}