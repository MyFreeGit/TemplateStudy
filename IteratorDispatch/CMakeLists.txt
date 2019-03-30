cmake_minimum_required(VERSION 3.4)
project(IteratorDispatch)

find_package(GTest REQUIRED)

include_directories(inc)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE UT_SRC ut/*.cpp)

add_executable(IteratorDispatchUnitTest ${UT_SRC})
target_link_libraries(IteratorDispatchUnitTest ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(IteratorDispatchUnitTest "" ${UT_SRC})

add_test(IteratorDispatch IteratorDispatchTest)