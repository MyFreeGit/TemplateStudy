#include "TypeList.hpp"
#include <gtest/gtest.h>
#include <type_traits>
#include <iostream>

namespace{
class TypeListTest: public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
        EXPECT_TRUE(sizeof(char) < sizeof(short));
        EXPECT_TRUE(sizeof(short) < sizeof(int));
    }
protected:
    using TEST_LIST = TypeList<char, short, int>;
    template<int v>
    using IC = std::integral_constant<int, v>;
};

template<typename T>
struct AddConstT
{
    using Type = const T;
};
template<typename T>
using addConst = typename AddConstT<T>::Type;

template<typename T1, typename T2>
struct LargerTypeT
{
    using Type = ifThenElse<(sizeof(T1) > sizeof(T2)), T1, T2>;
};
template<typename T1, typename T2>
using largerType = typename LargerTypeT<T1, T2>::Type;

template<typename T1, typename T2>
struct LargerSizeT
{
    static constexpr bool Value = sizeof(T1) > sizeof(T2);
};

template<typename T1, typename T2>
using largerSize = LargerSizeT<T1, T2>;

TEST_F(TypeListTest, popFront)
{
    EXPECT_TRUE((std::is_same<getHead<TEST_LIST>, char>::value));
    EXPECT_TRUE((std::is_same<getHead<TypeList<int>>, int>::value));
    EXPECT_TRUE((std::is_same<popFront<TypeList<int>>, TypeList<>>::value));
    EXPECT_TRUE((std::is_same<getHead<TypeList<>>, void>::value));
    EXPECT_TRUE((std::is_same<popFront<TypeList<>>, TypeList<>>::value));
}

TEST_F(TypeListTest, pushFront)
{
    EXPECT_TRUE((std::is_same<pushFront<TEST_LIST, float>, TypeList<float, char, short, int>>::value));
    EXPECT_TRUE((std::is_same<pushFront<popFront<TEST_LIST>, float>, TypeList<float, short, int>>::value));
}

TEST_F(TypeListTest, getAt)
{
    EXPECT_TRUE((std::is_same<getAt<TEST_LIST, 0>, char>::value));
    EXPECT_TRUE((std::is_same<getAt<TEST_LIST, 1>, short>::value));
    EXPECT_TRUE((std::is_same<getAt<TEST_LIST, 2>, int>::value));
}

TEST_F(TypeListTest, ifThenElse)
{
    EXPECT_TRUE((std::is_same<ifThenElse<true, long, int>, long>::value));
    EXPECT_TRUE((std::is_same<ifThenElse<false, long, int>, int>::value));
}

TEST_F(TypeListTest, getMaxSizeType)
{
    EXPECT_TRUE((std::is_same<getMaxSizeType<TypeList<char, short, int>>, int>::value));
    EXPECT_TRUE((std::is_same<getMaxSizeType<TypeList<char, int, short>>, int>::value));
    EXPECT_TRUE((std::is_same<getMaxSizeType<TypeList<int, short, char>>, int>::value));
}

TEST_F(TypeListTest, isEmpty)
{
    EXPECT_TRUE(isEmpty<TypeList<>>::value);
    EXPECT_TRUE(isEmpty<TypeList<int>>::value == false);
}

TEST_F(TypeListTest, pushBack)
{
    EXPECT_TRUE((std::is_same<pushBack<TypeList<>, int>, TypeList<int>>::value));
    EXPECT_TRUE((std::is_same<pushBack<TypeList<char>, int>, TypeList<char, int>>::value));
}

TEST_F(TypeListTest, pushBackV2)
{
    EXPECT_TRUE((std::is_same<pushBackV2<TypeList<>, int>, TypeList<int>>::value));
    EXPECT_TRUE((std::is_same<pushBackV2<TypeList<char>, int>, TypeList<char, int>>::value));
}

TEST_F(TypeListTest, reverse)
{
    EXPECT_TRUE((std::is_same<reverse<TEST_LIST>, TypeList<int, short, char>>::value));
}

TEST_F(TypeListTest, transform)
{
    EXPECT_TRUE((std::is_same<transform<TEST_LIST, addConst>, TypeList<const char, const short, const int>>::value));
}

TEST_F(TypeListTest, accumulate)
{
    EXPECT_TRUE((std::is_same<accumulate<TypeList<char, short, int>, largerType>, int>::value));
    EXPECT_TRUE((std::is_same<accumulate<TypeList<char, int, short>, largerType>, int>::value));
    EXPECT_TRUE((std::is_same<accumulate<TypeList<int, short, char>, largerType>, int>::value));
    EXPECT_TRUE((std::is_same<accumulate<TEST_LIST, pushFront, TypeList<>>, TypeList<int, short, char>>::value));
}

TEST_F(TypeListTest, insertSort)
{
    EXPECT_TRUE((std::is_same<sortList<TypeList<char, long long, int, short>, largerSize>,
                              TypeList<long long, int, short, char>
                             >::value));
}

TEST_F(TypeListTest, getFirstIndexOf)
{
    EXPECT_TRUE((getFirstIndexOf<char, TEST_LIST> == 0));
    EXPECT_TRUE((getFirstIndexOf<short, TEST_LIST> == 1));
    EXPECT_TRUE((getFirstIndexOf<int, TEST_LIST> == 2));
    // When the target type isn't in the type list, the compilation will fail! It is type safe in meta-programming :-)
    // EXPECT_TRUE((getFirstIndexOf<float, TEST_LIST> == 2));
}

TEST_F(TypeListTest, getLengthOf)
{
    EXPECT_TRUE((getLengthOf<TEST_LIST> == 3));
    EXPECT_TRUE((getLengthOf<popFront<TEST_LIST>> == 2));
    EXPECT_TRUE((getLengthOf<popFront<popFront<TEST_LIST>>> == 1));
    EXPECT_TRUE((getLengthOf<TypeList<>> == 0));
}

TEST_F(TypeListTest, getIntegralList)
{
    EXPECT_TRUE((std::is_same<getIntegralList<int, 0, 1, 3>, 
                              TypeList<IC<0>, IC<1>, IC<2>>
                             >::value));
    EXPECT_TRUE((std::is_same<getIntegralList<int, 5, 1, 1>, TypeList<IC<5>>>::value));
    EXPECT_TRUE((std::is_same<getIntegralList<int, 5, 1, 0>, TypeList<>>::value));}
}

TEST_F(TypeListTest, reverseV2)
{
    EXPECT_TRUE((std::is_same<reverseV2<TEST_LIST>, TypeList<int, short, char>>::value));
}
