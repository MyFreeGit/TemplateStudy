cmake_minimum_required(VERSION 3.0.0)
project(BasicKnowledge VERSION 0.1.0)

find_package(spdlog)
if (NOT spdlog_FOUND)
    message(WARNING "spdlog isn't found. STD iostream is used")
    add_definitions(-D_NO_SPDLOG)
endif (NOT spdlog_FOUND)

# Open it, when you don't need function name tracing (for example in Gtest framework.)
#add_definitions(-D_NO_FUNC_TRACING)

enable_testing()
include_directories(./ ./Tracer)
add_subdirectory(Tracer)
add_executable(JustATest main.cpp)
target_link_libraries(JustATest tracerLib)
add_subdirectory(UT)
