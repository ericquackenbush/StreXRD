cmake_minimum_required(VERSION 3.0)
project(archytas)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(include)

add_executable(header ${PROJECT_SOURCE_DIR}/src/io/header.cpp)