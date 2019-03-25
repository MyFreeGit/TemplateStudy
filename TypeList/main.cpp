#include <iostream>
#include <type_traits>
#include "TypeList.hpp"

int main(int, char**) 
{
    using test_list = TypeList<int, char, long, float, double>;
    if (std::is_same<popFront<test_list>, int>::value)
    {
        std::cout << "The First type in test_list is int" << std::endl;
    }

    return 1;
}
