cmake_minimum_required(VERSION 3.10)

project(NAMEC LANGUAGES CXX)

# LOAD CONAN DEPS
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

option(BUILD_TESTS OFF)

if(BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(src/client)
add_subdirectory(src/ai)


