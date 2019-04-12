#include <type_traits>
#include <iostream>
#include <boost/type_index.hpp>

using std::cout;
using std::endl;

// Simulate auto param = value; Simplify the code writing by no need to define different variables
auto checkAutoDecduction = [](auto param){
    using Type = decltype(param);
    using boost::typeindex::type_id_with_cvr;
    cout << "param's type is: "<< type_id_with_cvr<decltype(param)>().pretty_name() << endl;
};

// Simulate const auto param = value;
auto checkConstAutoDecduction = [](const auto param){
    using Type = decltype(param);
    using boost::typeindex::type_id_with_cvr;
    cout << "param's type is: "<< type_id_with_cvr<decltype(param)>().pretty_name() << endl;
};

// Simulate auto& param = value;
auto checkAutoReferenceDecduction = [](auto& param){
    using Type = decltype(param);
    using boost::typeindex::type_id_with_cvr;
    cout << "param's type is: "<< type_id_with_cvr<decltype(param)>().pretty_name() << endl;
};

// Simulate const auto& param = value;
auto checkConstAutoReferenceDecduction = [](const auto& param){
    using Type = decltype(param);
    using boost::typeindex::type_id_with_cvr;
    cout << "param's type is: "<< type_id_with_cvr<decltype(param)>().pretty_name() << endl;
};

template<typename T>
void checkTemplateTypeAndParamType(T param)
{
    using boost::typeindex::type_id_with_cvr;
    cout << "T is " << type_id_with_cvr<T>().pretty_name();
    cout << "; param's type is: " << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
}

template<typename T>
void checkTemplateReferenceAndParamType(T& param)
{
    using boost::typeindex::type_id_with_cvr;
    cout << "T is " << type_id_with_cvr<T>().pretty_name();
    cout << "; param's type is: " << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
}

template<typename T>
void checkTemplateConstReferenceAndParamType(const T& param)
{
    using boost::typeindex::type_id_with_cvr;
    cout << "T is " << type_id_with_cvr<T>().pretty_name();
    cout << "; param's type is: " << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
}

void showInputs()
{
    cout << "int i = 10;" << endl
         << "int& ir = i;" << endl
         << "const int ci = 10;" << endl
         << "const int& cir = ci;" << endl;
}

// show type of variable's type which is defined by:
// auto param = value;
void showAutoDeductionResult()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "auto param = i;   // ";
    checkAutoDecduction(i);
    cout << "auto param = 10;  // ";
    checkAutoDecduction(10);
    cout << "auto param = ci;  // ";
    checkAutoDecduction(ci);
    cout << "auto param = ir;  // ";
    checkAutoDecduction(ir);
    cout << "auto param = cir; // ";
    checkAutoDecduction(cir);
}

// show type of variable's type which is defined by:
// const auto param = value;
void showConstAutoDeductionResult()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "const auto param = i;   // ";
    checkConstAutoDecduction(i);
    cout << "const auto param = 10;  // ";
    checkConstAutoDecduction(10);
    cout << "const auto param = ci;  // ";
    checkConstAutoDecduction(ci);
    cout << "const auto param = ir;  // ";
    checkConstAutoDecduction(ir);
    cout << "const auto param = cir; // ";
    checkConstAutoDecduction(cir);
}

// show type of variable's type which is defined by:
// auto& param = value;
void showAutoReferenceDecductionResult()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "auto& param = i;   // ";
    checkAutoReferenceDecduction(i);
    // We cannot assign an RValue to a reference
    cout << "auto& param = 10;  // compiling will fail here" << endl;
    cout << "auto& param = ci;  // ";
    checkAutoReferenceDecduction(ci);
    cout << "auto& param = ir;  // ";
    checkAutoReferenceDecduction(ir);
    cout << "auto& param = cir; // ";
    checkAutoReferenceDecduction(cir);
}

// show type of variable's type which is defined by:
// const auto& param = value;
void showConstAutoReferenceDecductionResult()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    showInputs();
    cout << "const auto& param = i;   // ";
    checkConstAutoReferenceDecduction(i);
    cout << "const auto& param = 10;  //";
    checkConstAutoReferenceDecduction(10);
    cout << "const auto& param = ci;  // ";
    checkConstAutoReferenceDecduction(ci);
    cout << "const auto& param = ir;  // ";
    checkConstAutoReferenceDecduction(ir);
    cout << "const auto& param = cir; // ";
    checkConstAutoReferenceDecduction(cir);
}

void showTempalateParameterDeduction()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "template<typename T>" << endl;
    cout << "void func(T param);" << endl;

    cout << "func(i);   // ";
    checkTemplateTypeAndParamType(i);
    cout << "func(10);  // ";
    checkTemplateTypeAndParamType(10);
    cout << "func(ci);  // ";
    checkTemplateTypeAndParamType(ci);
    cout << "func(ir);  // ";
    checkTemplateTypeAndParamType(ir);
    cout << "func(cir); // ";
    checkTemplateTypeAndParamType(cir);
}

void showTempalateReferenceParameterDeduction()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "template<typename T>" << endl;
    cout << "void func(T& param);" << endl;

    cout << "func(i);   // ";
    checkTemplateReferenceAndParamType(i);
    cout << "func(10);  // Compile will failed here, due to cannot assign a RValue to reference" << endl;
    cout << "func(ci);  // ";
    checkTemplateReferenceAndParamType(ci);
    cout << "func(ir);  // ";
    checkTemplateReferenceAndParamType(ir);
    cout << "func(cir); // ";
    checkTemplateReferenceAndParamType(cir);
}

void showTempalateConstReferenceParameterDeduction()
{
    int i = 10;
    int& ir = i;
    const int ci = 10;
    const int& cir = ci;

    cout << "template<typename T>" << endl;
    cout << "void func(const T& param);" << endl;

    cout << "func(i);   // ";
    checkTemplateConstReferenceAndParamType(i);
    cout << "func(10);  // ";
    checkTemplateConstReferenceAndParamType(10);
    cout << "func(ci);  // ";
    checkTemplateConstReferenceAndParamType(ci);
    cout << "func(ir);  // ";
    checkTemplateConstReferenceAndParamType(ir);
    cout << "func(cir); // ";
    checkTemplateConstReferenceAndParamType(cir);
}

int main(int, char**)
{
    showInputs();
    cout << endl;
    showAutoDeductionResult();
    cout << endl;
    showConstAutoDeductionResult();
    cout << endl;
    showAutoReferenceDecductionResult();
    cout << endl;
    showConstAutoReferenceDecductionResult();
    cout << endl;

    showTempalateParameterDeduction();
    cout << endl;
    showTempalateReferenceParameterDeduction();
    cout << endl;
    showTempalateConstReferenceParameterDeduction();
    cout << endl;
    return 1;
}