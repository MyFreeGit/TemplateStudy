cmake_minimum_required(VERSION 3.4)
project(TupleTest)

find_package(GTest REQUIRED)

include_directories(inc)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UT_SRC ut/*.cpp)

add_executable(TupleTest ${SOURCES})

add_executable(TupleUnitTest ${UT_SRC})
target_link_libraries(TupleUnitTest ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(TupleUnitTest "" ${UT_SRC})

add_test(TupleTest TupleUnitTest)