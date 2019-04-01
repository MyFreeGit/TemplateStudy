find_package(GTest REQUIRED)

file(GLOB SOURCES ./*.cpp)
include_directories(./)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(TypeListTest ${SOURCES})
target_link_libraries(TypeListTest ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(TypeListTest "" ${SOURCES})