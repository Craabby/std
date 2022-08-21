cmake_minimum_required(VERSION 3.5.0)
project(std)

include_directories(include)

set(SRCS
    src/test.cc)

set(CMAKE_CXX_FLAGS "-g")

add_executable(std ${SRCS})
