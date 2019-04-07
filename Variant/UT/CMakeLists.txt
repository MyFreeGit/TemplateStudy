file(GLOB SOURCES ./*.cpp)
add_executable(VariantTest ${SOURCES})
enable_testing()

include_directories(./)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(VariantTest ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(VariantTest "" ${SOURCES})