file(GLOB SOURCES ./*.cpp)
include_directories(./)
add_library(tracerLib STATIC ${SOURCES})