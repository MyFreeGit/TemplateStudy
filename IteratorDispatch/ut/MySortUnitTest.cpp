#include <gtest/gtest.h>
#include <list>
#include <vector>

#include "MySort.hpp"

namespace{
class MySortUnitTest: public ::testing::Test
{
protected:
    using IntList = std::list<int>;
    using IntVec = std::vector<int>;
};

TEST_F(MySortUnitTest, SortWithRawIntList)
{
    int list[] = {6, 7, 3, 2, 1, 4};
    IntList result{1, 2, 3, 4, 6, 7};
    my_sort(list, list + 6);
    IntList l(list, list+ 6);
    EXPECT_EQ(l, result);
}

TEST_F(MySortUnitTest, SortWithIntList)
{
    IntList list{6, 7, 3, 2, 1, 4};
    IntList result{1, 2, 3, 4, 6, 7};
    my_sort(list.begin(), list.end());
    EXPECT_EQ(list, result);
}

TEST_F(MySortUnitTest, SortWithIntVec)
{
    IntVec list{6, 7, 3, 2, 1, 4};
    IntVec result{1, 2, 3, 4, 6, 7};
    my_sort(list.begin(), list.end());
    EXPECT_EQ(list, result);
}
}