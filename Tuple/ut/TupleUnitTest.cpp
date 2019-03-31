#include <gtest/gtest.h>
#include <iostream>
#ifdef MY_TUPLE_VERSION_2
#include "MyTupleV2.hpp"
#else
#include "MyTuple.hpp"
#endif
namespace{
class TupleUnitTest: public ::testing::Test
{
protected:
};

TEST_F(TupleUnitTest, WithOneIntValue)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    Tuple<int> tupleWithRValue(5);
    EXPECT_EQ(5, getValue<0>(tupleWithRValue));
    // Test Tuple with lvalue
    int i = 10;
    Tuple<int> tupleWithLValue(i);
    EXPECT_EQ(10, getValue<0>(tupleWithLValue));
}

TEST_F(TupleUnitTest, WithThreeValues)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    Tuple<int, float, std::string> t(5, 5.6, "Hello World");
    EXPECT_EQ(5, getValue<0>(t));
    EXPECT_FLOAT_EQ(5.6, getValue<1>(t));
    EXPECT_STREQ("Hello World", getValue<2>(t).c_str());
    int i = 10;
    Tuple<int, float, std::string> tupleWithMixReference(i, 5.6, "Hello World");
}

TEST_F(TupleUnitTest, testMakeTuple)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    using MyStudy::makeTuple;
    auto t = makeTuple(5, 5.6, std::string("Hello World"));
    EXPECT_EQ(5, getValue<0>(t));
    EXPECT_FLOAT_EQ(5.6, getValue<1>(t));
    EXPECT_STREQ("Hello World", getValue<2>(t).c_str());
}

TEST_F(TupleUnitTest, testEqualOperator)
{
    using MyStudy::Tuple;
    using MyStudy::makeTuple;
    auto t1 = makeTuple(5, 5.5, std::string("Hello"));
    auto t2 = t1;
    auto t3 = makeTuple(6, 5.5, std::string("A"));
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 == t3);
    auto t4 = t1.getTails();
    EXPECT_TRUE(t4 == makeTuple(5.5, std::string("Hello")));
    //auto t4 = makeTuple(7, std::string("A"), 7.8);
    // t1 == t4 // This cannot be compiled! (Due to t1 and t4 are different data type!)
}
}


