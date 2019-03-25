cmake_minimum_required(VERSION 3.0.0)
project(FunctorExample VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(./inc)
add_executable(FunctorStep1 ./step1/BasicFunctor.cpp)

find_package(GTest REQUIRED)

add_executable(FunctorStep2 ./step2/FunctorWithVariadicTemplate.cpp)

add_executable(FunctorStep3 ./step3/FullFunctor.cpp)

add_executable(FunctorStep4 ./step4/FinalFunctor.cpp)