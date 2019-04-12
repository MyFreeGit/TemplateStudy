find_package(spdlog)
if (NOT spdlog_FOUND)
    message(WARNING "spdlog isn't found. STD iostream is used")
    add_definitions(-D_NO_SPDLOG)
endif (NOT spdlog_FOUND)

# Open it, when you don't need function name tracing (for example in Gtest framework.)
#add_definitions(-D_NO_FUNC_TRACING)

file(GLOB SOURCES ./*.cpp)
include_directories(./)
add_library(tracerLib STATIC ${SOURCES})