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

TEST_F(TupleUnitTest, TupleMemberIsMutable)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    Tuple<int> t(5);
    EXPECT_EQ(5, getValue<0>(t));
    int& l = getValue<0>(t);
    l = 20;
    EXPECT_EQ(20, getValue<0>(t));
}

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
    // Test Tuple with reference
    int& r = i;
    Tuple<int> tupleWithReference(i);
    r = 20;
    EXPECT_EQ(10, getValue<0>(tupleWithReference)); // Not changed inside the Tuple
}

TEST_F(TupleUnitTest, CopyConstructor)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    using MyStudy::makeTuple;
    auto t = makeTuple(5, 5.6, std::string("Hello World"));
    Tuple<int, double, std::string> t2 = t;
    EXPECT_EQ(5, getValue<0>(t));
    EXPECT_FLOAT_EQ(5.6, getValue<1>(t));
    EXPECT_STREQ("Hello World", getValue<2>(t).c_str());

    EXPECT_EQ(5, getValue<0>(t2));
    EXPECT_FLOAT_EQ(5.6, getValue<1>(t2));
    EXPECT_STREQ("Hello World", getValue<2>(t2).c_str());
}

TEST_F(TupleUnitTest, MoveConstructor)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    using MyStudy::makeTuple;
    auto t = makeTuple(5, 5.6, std::string("Hello World"));
    Tuple<int, double, std::string> t2 = std::move(t);
    EXPECT_EQ(5, getValue<0>(t));
    EXPECT_FLOAT_EQ(5.6, getValue<1>(t));
    EXPECT_EQ(getValue<2>(t).length(), 0);

    EXPECT_EQ(5, getValue<0>(t2));
    EXPECT_FLOAT_EQ(5.6, getValue<1>(t2));
    EXPECT_STREQ("Hello World", getValue<2>(t2).c_str());
}

TEST_F(TupleUnitTest, EBCOImprovments)
{
    using MyStudy::Tuple;
    struct E1{};
    struct E2{};
    struct E3 final {};
    Tuple<int, E1, E2> t1(10, E1(), E2());
    EXPECT_EQ(sizeof(t1), sizeof(int));

    Tuple<int, E1, E3> t2(10, E1(), E3());
    //Empty class only cost 1 byte space.
    EXPECT_EQ((std::alignment_of<E3>()), 1);
    // E3 is final cannot be derived. So the size of t2 is total space that is needed for int and E3. Due to the
    // CPP alignment, the member data of E3 needs to be aligned with int, so the total size of t2 is 4 * 2.
    // Compare to t1, the EBCO can save the space that required by alignment when the member data's type is empty class.
    EXPECT_EQ(sizeof(t2),  (std::alignment_of<Tuple<int, E1, E3>>() * 2));
}

TEST_F(TupleUnitTest, TupleMovableContructor)
{
    using MyStudy::Tuple;
    using MyStudy::getValue;
    std::string str = "Hello World";
    Tuple<std::string> t(std::move(str));
    EXPECT_STREQ("Hello World", getValue<0>(t).c_str());
    EXPECT_EQ(str.length(), 0);
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
    //auto t4 = makeTuple(7, std::string("A"), 7.8);
    // t1 == t4 // This cannot be compiled! (Due to t1 and t4 are different data type!)
}
}


