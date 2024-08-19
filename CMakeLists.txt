cmake_minimum_required(VERSION 3.30)
project("leet")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 99)

add_subdirectory("include")

if(NOT DEFINED ENV{NO_BENCHMARKS})
    add_subdirectory("benchmarks")
endif()

if(NOT DEFINED ENV{NO_DOCS})
    add_subdirectory("docs")
endif()

enable_testing()
add_subdirectory("tests")
