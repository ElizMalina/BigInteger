cmake_minimum_required(VERSION 3.10)

project(BigInteger)

set (CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(googletest https://github.com/google/googletest.git)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(my_program biginteger.cpp decode.cpp)

add_executable(my_tests tests.cc)

target_link_libraries(my_tests gtest gtest_main)