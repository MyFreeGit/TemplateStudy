#include <iostream>
#include "FinalFunctor.hpp"
#include <functional>

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

int main(int, char**)
{
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
    // The compiler fails when deduce the Functor's template arguments. The Functor
    // internal data type isn't a int(void) type and use a bridge pattern to record the callable object. But
    // the int(void) type gurantee the correct type is assigned to functor object!
    //Functor<int(void)> shall_fail = 15;
}