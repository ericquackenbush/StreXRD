cmake_minimum_required(VERSION 3.0)

project(read_mar)

SET(SOURCES read_mar.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(read_mar ${SOURCES})
target_link_libraries(read_mar ${OpenCV_LIBS})