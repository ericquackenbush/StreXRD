cmake_minimum_required(VERSION 3.0)

project(image_object)

SET(SOURCES main.cpp image_object.cpp read_mar.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(image_object ${SOURCES})
target_link_libraries(image_object ${OpenCV_LIBS})