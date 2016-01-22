cmake_minimum_required(VERSION 3.3)
project(qr_decomposition)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp matrix.hpp nvector.hpp ops.hpp common.hpp)
add_executable(qr_decomposition ${SOURCE_FILES})