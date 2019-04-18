---
marp: true
title: Template Argument Deduction
---
# Template Argument Deduction
   - Auto keyword
   - Perfect Forwarding
---
# 理解Template和Variable的类型
   - 假设我们定义了一个模板函数
        ```C++
        template<typename T>  // T 为模板参数
        T max(T x, T y)  // x, y 函数参数 
        {
            return (x > y) ? x, y;
        }
        ```
   - 在Template Instantiation时
        ```C++
        auto x = max(10, 15); // 10，15或其它变量甚至表达式为函数入参
        ```
        为了方便使用模板函数，C++编译器提供了从函数传入参的类型推导出模板参数（T）的类型，并根据T的类型定义参数（x,y）类型的机制，称为Template Parameter Deduction。
        编译器需要从传入参推导出T的实际类型，用于生成代码。
          
 
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
      - Alias Template从不使用推导，使用时必须显示给出模板参数。
      - 模板类的参数不使用推导（C+17开始，可以通过类构造函数的入参推导类模板参数）
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
   具体可参见[argument_deduction_demo](https://github.com/MyFreeGit/TemplateStudy/tree/master/BasicKnowledge/argument_deduction_demo)

---
# 没有涉及的话题
   - 组合类型的参数推导
      ```C++
      int max(int x, int y);
      template<typename R, typename X， typename Y>
      func(R(X, Y) func);
      ...
      func(max);  // 从传入的函数指针中推导出， 模板参数R， X， Y的类型
      ```
   - Variadic Template Argument Deduction
      ```C++
      template<typename... Args>
      void print(Args... args);
      ...
      print(10, 3.14f); // 可变参数模板的参数展开后，再推导每一个参数
      ```
   - Initialization List 参数的推导
      ```C++
      template<class T> void f(std::initializer_list<T>);
      f({1, 2, 3});  // T 为int类型
      ```

---
# Forward Reference的使用场景
   - 先看一段经过简化的STL中make_shared函数的伪代码
      ```C++
      template<typename T, typename... Args>
      shared_ptr<T> make_shared(Args&&... args)
      {
         return shared_ptr<T>(new T(std::forward<Args>(args)...));
      }
      ```
      make_shared函数的主要作用就是调用类型T的构造函数，创建一个类型为T的对象，然后创建一个shared_ptr<T>的对象，并指向这   个新创建的对象。
      - 利用Variadic Template, 可以接收任意个数的参数.
      - 利用std::forward将入参传递给T的构造函数

---
# Forwarding Reference
   - 在make_shared的函数定义中
      ```C++
      template< class T, class... Args >
      shared_ptr<T> make_shared( Args&&... args);
      ```
      我们会看到模板参数Args&&，长的有些类似RValue Reference的定义。但Args&&是一个Forwarding Reference。
   - RValue Reference是一个具体的类型加上&&符号，比如
      ```C++
      int&& i = 10;
      std::vector<T>&& vec = someOtherVec; // vector<T> 虽然带有模板参数T，但它的类型是确定的！
      ```
   - 而Forward Reference是模板参数或auto加上&&符号，比如：
      ```C++
      template<typename T>
      void func(T&& param);
      auto&& ref = temp;
      ```
---
# 理解Forwarding Reference
   - 首先它是一个reference，所以它必须要指向一个对象。所以使用前必须被初始化。
   - 它的主要用途是Forward，将一个函数的入参Forward给另一个函数，以make_shared函数为例，它的主要用途就是创建一个shared_ptr指向一个新创建的对象，并将参数中的args传递给该对象的构造函数。
      - 显然从效率考虑，使用reference作为参数是最合理的选择。
      - 从使用角度考虑，make_shared的参数可以接受任何类型的参数（普通对象，Reference, RValue Reference）
      - 从正确性来考虑，经过make_shared的传递（入参 -> Forward Reference -> 构造函数的入参）构造函数得到的入参和make_shared的入参保持一致。即
         - Reference和Object经过传递，构造函数得到是Reference
         - RValue经过传递，构造函数得到还是RValue

---
# Reference Collapse与Perfect Forwarding
   - 在C++11之前，不允许一个reference引用另一个reference。T&&会引发编译错误。
   - C++11为实现Perfect Forward和RValue Reference。引入了Reference Collapse规则
     > T& &   becomes T&   //**T& &是引用另一个reference，而不是RValue Reference**
     > T& &&  becomes T&
     > T&& &  becomes T&
     > T&& && becomes T&&
   - Reference Collapse规则运用于Forward Reference和Perfect Forwarding的实现中。
      ```C++
      template<class T>
      T&& forward(typename remove_reference<T>::type& param) noexcept
      {
        return static_cast<T&&>(param); // 触发Reference Collapse
      } 
      ```
      如T类型为value 或 reference。 forward结果为reference。
      如果T类型为RValue reference。 forward结果为RValue Reference。

---
# 配合std::forward使用Forward Reference
   - 使用std::forward 传递Forward Reference
      ```C++
      template<typename T>
      void perfectForward(T&& param)
      {
          func(std::forward<T>(param)); // forward后必须显示给出模板类型！
      }
      ```
      C++编译器会根据函数perfectForward的入参类型，推导出param的类型后，调用对应的func函数（一般func函数是overload函数，不然就没有什么意义，不如直接定义参数param的类型和func的函数一致。）
   - 注意不要在对Forward Reference进行std::move操作！
      - 会导致传入func的参数类型混乱
      - 但对RValue Reference使用std::forward的效果和std::move一致

   具体可参见[perfect_forward_demo](https://github.com/MyFreeGit/TemplateStudy/tree/master/BasicKnowledge/perfect_forward_demo)

---
# Perfect Forward的使用场景
   - 实现 tuple, variant, any, option等容器类时，将构造函数中接收到参数Forward给内部封装类型的构造函数。
   - 实现消息的派发函数，将接受到的消息，派发到不同的模块中。
      - 5gnb的代码中有应用
   - 在实现Factory设计模式时，将Factory函数的入参传递给被创建对象的构造函数
      - std::make_shared
   - Lambda函数中使用结合decltype使用Perfect Forward
      ```C++
      [](auto&& param) { 
         targetFunc(std::forward<decltype(param)>(param)); 
      };
      ```

