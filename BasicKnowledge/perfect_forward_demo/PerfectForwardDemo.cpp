#include <type_traits>
#include <iostream>
#include <boost/type_index.hpp>

using std::cout;
using std::endl;

#define TRACE_FUNCTION_CALL()\
std::cout <<  __PRETTY_FUNCTION__ << " is called." << std::endl 

void func(int& i) { TRACE_FUNCTION_CALL(); }
void func(const int& i) { TRACE_FUNCTION_CALL(); }
void func(int&& i) { TRACE_FUNCTION_CALL(); }

template<typename T>
void checkForwardingReferenceType(T&& param)
{
    using boost::typeindex::type_id_with_cvr;
    cout << "T is " << type_id_with_cvr<T>().pretty_name();
    cout << "; param's type is: " << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
}

template<typename T>
void perfectForward(T&& param)
{
    func(std::forward<T>(param));
}

void showInputs()
{
    cout << "int i = 10;" << endl
         << "int& ir = i;" << endl
         << "const int ci = 10;" << endl
         << "const int& cir = ci;" << endl;
}

void showForwardingReferenceType()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "template<typename T>" << endl;
    cout << "void func(T&& param);" << endl;

    cout << "func(i);    // ";
    checkForwardingReferenceType(i);
    cout << "func(10);   // ";
    checkForwardingReferenceType(10);
    cout << "func(ci);   // ";
    checkForwardingReferenceType(ci);
    cout << "func(ir);   // ";
    checkForwardingReferenceType(ir);
    cout << "func(cir);  // ";
    checkForwardingReferenceType(cir);
}

void demoPerfectForward()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "perfectForward(i);    // ";
    perfectForward(i);
    cout << "perfectForward(10);   // ";
    perfectForward(10);
    cout << "perfectForward(ci);   // ";
    perfectForward(ci);
    cout << "perfectForward(ir);   // ";
    perfectForward(ir);
    cout << "perfectForward(cir);  // ";
    perfectForward(cir);
}

int main(int, char**)
{
    showInputs();
    cout << endl;
    showForwardingReferenceType();
    cout << endl;
    demoPerfectForward();
    return 1;
}