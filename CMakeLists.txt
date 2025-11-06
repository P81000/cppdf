cmake_minimum_required(VERSION 3.10)

project(cppdf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(cppdf ${SOURCES})

target_include_directories(cppdf PUBLIC include)
