#include <gtest/gtest.h>
#include <string>
#include "myVariant.hpp"

namespace{
using ::MyStudy::Variant;
using ::MyStudy::getValue;

class VariantTest: public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
    }
protected:
};

TEST_F(VariantTest, initWithRValue)
{
    Variant<int, float> v1(10);
    EXPECT_EQ(getValue<0>(v1), 10);
    EXPECT_EQ(getValue<int>(v1), 10);

    EXPECT_THROW({getValue<float>(v1);}, ::MyStudy::DataNotInBuffer);
    EXPECT_THROW({getValue<1>(v1);}, ::MyStudy::DataNotInBuffer);
    // The Meta programming can gurantee the compiling failure when Index or type isn't in Variant
    //EXPECT_THROW({getValue<2>(v1);}, ::MyStudy::DataNotInBuffer); //Compiling will failed here

    std::string s = "Hello World";
    Variant<int, std::string> v2(std::move(s));
    EXPECT_TRUE(getValue<1>(v2) == "Hello World");
    EXPECT_TRUE(s.empty());
}

TEST_F(VariantTest, initWithLValue)
{
    int i = 10;
    Variant<int, float> v(i);
    EXPECT_EQ(getValue<0>(v), 10);
    EXPECT_EQ(getValue<int>(v), 10);

    EXPECT_THROW({getValue<float>(v);}, ::MyStudy::DataNotInBuffer);
    EXPECT_THROW({getValue<1>(v);}, ::MyStudy::DataNotInBuffer);
}

TEST_F(VariantTest, assignment)
{
    Variant<int, float, std::string> v(10);
    v = 3.14f;
    EXPECT_FLOAT_EQ(getValue<float>(v), 3.14f);
    EXPECT_THROW({getValue<int>(v);}, ::MyStudy::DataNotInBuffer);

    std::string s = "Hello World";
    v = std::move(s);
    EXPECT_TRUE(getValue<2>(v) == "Hello World");
    EXPECT_TRUE(s.empty());
}
}