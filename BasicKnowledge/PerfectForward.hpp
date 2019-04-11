#include <utility>
#include "Tracer.hpp"

#pragma once

namespace MyStudy{
using string = std::string;
// For simplicity only allow int as type parameter 
template<typename T, typename = std::enable_if<std::is_same<std::decay_t<T>, int>::value>>
void checkTypeInForward(T&& p)
{
    if(std::is_same<T, int>::value)
    {
        std::cout << "Got int" << std::endl;
    }
    if(std::is_same<T, int&>::value)
    {
        std::cout << "Got int&" << std::endl;
    } 
    if(std::is_same<T, int&&>::value)
    {
        std::cout << "Got int&&" << std::endl;
    }
    if(std::is_same<T, const int>::value)
    {
        std::cout << "Got const int" << std::endl;
    }
    if(std::is_same<T, const int&>::value)
    {
        std::cout << "Got const int&" << std::endl;
    }
    if(std::is_same<T, const int&&>::value)
    {
        std::cout << "Got const int&&" << std::endl;
    }
    if(std::is_same<T, int*>::value)
    {
        std::cout << "Got const int*" << std::endl;
    }
}

template<typename T, typename = std::enable_if<std::is_same<std::decay_t<T>,int>::value>>
void checkTypeInNormalCall(T p)
{
    if(std::is_same<T, int>::value)
    {
        std::cout << "Got int" << std::endl;
    }
    if(std::is_same<T, int&>::value)
    {
        std::cout << "Got int&" << std::endl;
    } 
    if(std::is_same<T, int&&>::value)
    {
        std::cout << "Got int&&" << std::endl;
    }
}

}//namespace MyStudy