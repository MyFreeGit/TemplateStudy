find_package(GTest REQUIRED)

file(GLOB SOURCES ./*.cpp)
include_directories(./)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(TestForward ${SOURCES})
target_link_libraries(TestForward ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(TestForward "" ${SOURCES})