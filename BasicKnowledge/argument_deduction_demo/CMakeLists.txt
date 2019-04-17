find_package(boost REQUIRED)
file(GLOB SOURCES ./*.cpp)
include_directories(./)
add_executable(ArgumentDeductionDemo ${SOURCES})