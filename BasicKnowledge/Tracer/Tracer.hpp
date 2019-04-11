#include <string>
#include <iostream>
#ifndef _NO_SPDLOG
#include <spdlog/spdlog.h>
#endif

#pragma once

#ifndef _NO_SPDLOG
#define TRACE_FUNCTION_CALL()\
    spdlog::info("{} is called.",__PRETTY_FUNCTION__)
#else
    #define TRACE_FUNCTION_CALL()\
    std::cout <<  __PRETTY_FUNCTION__ << " is called." << std::endl 
#endif

namespace MyStudy{
using string = std::string;

// Tracer is used for Testing how the object is create and destoried. It is useful
// to check how the object is handled in template.
class Tracer
{
public:
    // Default Constructor
    Tracer(int count=0) : value(std::to_string(count))
    {
        TRACE_FUNCTION_CALL();
        defaultContructor++; 
    }

    // Copy Constructor
    Tracer(const Tracer& t) : value(t.value)
    {
        TRACE_FUNCTION_CALL();
        copyContructor++;
    }

    // Move Constructor
    Tracer(Tracer&& t) : value(std::move(t.value))
    {
        TRACE_FUNCTION_CALL();
        moveContructor++; 
    }

    // Copy assignment Operator
    Tracer& operator = (const Tracer& t) 
    {
        TRACE_FUNCTION_CALL();
        this->value = t.value;
        copyAssignment++;
        return *this;
    }

    // Move assignment Operator
    Tracer& operator = (Tracer&& t) 
    {
        TRACE_FUNCTION_CALL();
        this->value = std::move(t.value);
        moveAssignment++;
        return *this;
    }

    operator const string () const { return value; }

    // Destructor
    ~Tracer()
    {
        TRACE_FUNCTION_CALL();
        destructor++;
    }

    long getContructorCalledNum() { return defaultContructor; }
    long getCopyContructorCalledNum() { return copyContructor; }
    long getMoveContructorCalledNum() { return moveContructor; }
    long getCopyAssignmentCalledNum() { return copyAssignment; }
    long getMoveAssignmentCalledNum() { return moveAssignment; }
    long getDestructorCalledNum() { return defaultContructor; }

    void reset() 
    {
        defaultContructor = 0;
        moveContructor = 0;
        copyContructor = 0;
        destructor = 0;
        copyAssignment = 0;
        moveAssignment = 0;
    }
private:
    std::string value; // We use string to check the move affect
    // Using C++17 you can use inline static long XXX = 0; to do in-class initialization
    static long defaultContructor;     // Number of constructor called
    static long moveContructor;        // Number of move constructor called
    static long copyContructor;        // Number of copy constructor called
    static long destructor;            // Number of destructor called
    static long copyAssignment;        // Number of assignment
    static long moveAssignment;        // Number of move construc
};
} // namespace MyStudy
