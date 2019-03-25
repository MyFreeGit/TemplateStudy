#include <iostream>
#include "MyTuple.hpp"


int main()
{
    using namespace MyStudy;
    auto t = makeTuple(1, "Hello World");
    std::cout << "(" <<getValue<0>(t) << ", " << getValue<1>(t) << ")" << std::endl;
    return 1;
}