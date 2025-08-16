cmake_minimum_required(VERSION 3.14)

project(BigInteger)

set (CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/heads/main.zip)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(my_program biginteger.cpp main.cpp)

add_executable(my_tests tests.cc)

add_library(BigInteger biginteger.cpp)

target_link_libraries(my_program PRIVATE BigInteger)

target_link_libraries(my_tests PRIVATE BigInteger GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME my_tests COMMAND my_tests)
