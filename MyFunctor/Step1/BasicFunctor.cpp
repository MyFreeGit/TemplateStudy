#include <iostream>
#include "FunctorWithoutTemplate.hpp"

using AddFunc = int(*)(int, int);

int main(int, char**) {
    Functor f1 = AddFunctor();
    Functor f2 = normalAddFunction;
    f1(3,4);
    f2(5,6);
    // int start = 0;
    // AddFunc f = [start=](int i, int j)->int {return i + j;};
    // f(1,2);
}