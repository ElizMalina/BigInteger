cmake_minimum_required(VERSION 3.10)

project(BigInteger)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES biginteger.cpp decode.cpp)

add_executable(my_program ${SOURCES})