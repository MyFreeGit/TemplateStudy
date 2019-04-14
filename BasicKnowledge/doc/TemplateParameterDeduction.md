---
marp: true
title: Template Parameter Deduction
---
# Template Parameter Deduction
   - Auto keyword
   - Perfect Forwarding
---
# 理解Template和Variable的类型
   - 假设我们定义了一个模板函数
        ```C++
        template<typename T>
        T max(T x, T y)
        {
            return (x > y) ? x, y;
        }
        ```
   - 在Template Instantiation时
        ```C++
        auto x = max(10, 15); // 也可显示定义： float x = max<float>(5, 8);
        ```
        为了方便使用模板函数，C++编译器提供了从函数传入的参数的类型推导出模板参数（T）的类型，并根据T的类型定义参数（x,y）类型。
        编译器需要从传入的参数（例子中的10,15或变量，甚至表达式中）推导出T的实际类型，用于生成代码。
        
 
---
# 为什么我们要关心Template和Variable的类型
   - C++是强类型语言，如果不清楚Variable的类型，我们的代码会遇上各类问题：
       - 类型之间是否可以转换,赋值
       - 变量的值是否可以修改
       - 成员函数是否可以访问
       - 代码的效率如何
       ...
   - 在实现通用的模板函数或类时,需要对模板参数的类型进行些限制
      - 模板类型是否是实现了某一接口（指定基类的子类）
      - 针对模板类型进行优化 （Template Specialization）
      - 使用Type Traits获得类型的特征

---
# 如何推导Template的类型 (T不是reference时)
   - 对于 
      ```C++
      template<typename T>
      void func(T param);
      ```
      为了统一描述，只针对int 类型的参数。
      ```C++
      int i = 10;
      int& ir = i;       // r代表reference
      const int ci = 10; // c代表const
      const int& cir = ci;
      ```
      当模板函数func接受以上变量作为参数时，首先**忽略传入变量的（const， reference）剩下的类型就为T的类型**，此例中为int。

---
# 如何推导Template的类型 (当T为reference时)
   - 当模板函数定义为：
      ```C++
      template<typename T>
      void func(T& param);
      ```
      传入前面例子中的参数时，使用相同规则。**在忽略传入变量的reference后，得到T的类型**为int，或 const int
      ```C++
      func(i);   // 忽略i的reference，剩下int， T为int
      func(ir);  // 忽略ir的reference，剩下int， T为int
      func(ci);  // 忽略ci的reference，剩下const int， T为const int
      func(cir); // 忽略cir的reference，剩下const int，T为const int
      ```
---
# 如何推导Template的类型 (当T为const reference时)
   - 当模板函数定义为：
     ```C++
     template<typename T>
     void func(const T& param);
     ```
     传入前面例子中的参数时，使用相同规则。**在忽略传入变量的 const 和 reference后，得到T的类型**为int

---
# 根据Template的类型定义参数的类型
   - 当编译器得到T的类型后，可以定义出param的类型：
     |  | Template类型 | 参数类型 |
     | -- | ------------ | ------- |
     | T  | int | int |
     | T& | int | int& |
     | T& | const int| const int& |
     | const T& | int | const int& |
   
---
# 总结Template和变量的参数类型推导
   - 对于Template参数类型，忽略reference，只可能为int 或 const int类型。 如果传入参数类型中带有violate，同样和reference一样忽略。
   - 在模板参数推导的过程中，不考虑任何的构造函数和类型转换操作符的影响。
   - **推导规则只针对参数推导和auto类型定义**
   - 当显示的给出模板参数时，编译器不会使用推导。此时出现变量类型不匹配，编译器会调用合适的构造函数和类型转换操作符。
      ```C++
      max<float>(5, 6); // 编译器会将5, 6转化成float类型的值传给x, y。
      ```
---
# 理解 auto 变量的类型
   - **auto 变量必须初始化**。不然编译器没有办法从初始化语句的结果中推导出auto 变量的类型
   - auto 变量的类型定义规则和函数模板参数的定义规则一致。 可以简单将auto看作模板参数。然后套用模板函数参数类型推导方法，得到auto 变量的类型。
   ```C++
   auto v = ...       // 普通非const， 非reference的auto变量
   const auto v = ... // const 非reference的auto变量（由赋值语句的结果是否为const决定）
   auto& v = ...      // 指向普通或const对象的reference
   const auto& v = ... // 指向const 对象的reference
   ```
---
# 考察C++代码中数据类型的一些技巧
   - 查看编译出错时，编译器的输出。
   - 利用IDE。比如Visual Studio的Intelli Sense可以推导出当前auto变量或模板参数的类型。
   - 利用STL或boost的库，在代码中加入log用于打印模板参数类型或auto变量的类型
   ```C++
   #include <boost/type_index.hpp>
   ...
   auto v = ...
   // 先利用decltype(v)得到auto变量的类型，再调用boost库得到包含类型描述的字符串
   boost::typeindex::type_id_with_cvr<decltype(v)>().pretty_name();
   template<typename T>
   void func(T param)
   {
        boost::typeindex::type_id_with_cvr<T>().pretty_name();
   }
   ```
   具体可参见[parameter_deduce_demo](https://github.com/MyFreeGit/TemplateStudy/tree/master/BasicKnowledge/parameter_deduce_demo)


