#include <iostream>
#include <type_traits>
#include <PerfectForward.hpp>

using namespace MyStudy;
int main(int, char**) 
{
    int i = 10;
    checkTypeInForward(i);
    checkTypeInForward(10);
    checkTypeInForward(std::forward<int>(i));
    checkTypeInForward(std::forward<int&>(i));
    const int ci = 10;
    checkTypeInForward(ci);
    checkTypeInForward(std::forward<const int>(i));
    int & ri = i;
    checkTypeInForward(ri);
    checkTypeInForward(&ri);
    return 1;
}
