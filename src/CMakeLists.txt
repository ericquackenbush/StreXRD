cmake_minimum_required (VERSION 2.8)
project (test)
 
file(GLOB SOURCES
    header-folder/*.h
    source-folder/*.cpp
)

add_executable(test ${SOURCES})