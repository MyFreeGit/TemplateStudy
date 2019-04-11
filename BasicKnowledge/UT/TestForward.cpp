#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <type_traits>
#include <iostream>

namespace{
class TestForward: public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
    }
protected:
};

TEST_F(TestForward, popFront)
{
}

}//namespace