cmake_minimum_required(VERSION 3.4)
project(TupleTest)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UT_SRC ut/*.cpp)

add_executable(TupleUnitTest ${UT_SRC})
enable_testing()

set (TestTupleVersion2 TRUE)

if (TestTupleVersion2)
    target_include_directories(TupleUnitTest PRIVATE ../TypeList)
    target_compile_definitions(TupleUnitTest PRIVATE MY_TUPLE_VERSION_2)
else(TestTupleVersion2)
    target_include_directories(TupleUnitTest PRIVATE ./tuple_v1)
endif (TestTupleVersion2)

target_include_directories(TupleUnitTest PRIVATE ./inc)
target_link_libraries(TupleUnitTest ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(TupleUnitTest "" ${UT_SRC})

