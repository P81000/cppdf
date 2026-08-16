cmake_minimum_required(VERSION 3.20)

project(cppdf
    VERSION     0.1.0
    DESCRIPTION "Terminal-first PDF viewer with Vim keybindings"
    LANGUAGES   CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Build profile: Debug | Release | Profile | TSan" FORCE)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release Profile TSan)

foreach(_cfg Profile TSan)
    string(TOUPPER ${_cfg} _CFG)
    set(CMAKE_CXX_FLAGS_${_CFG}           "" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_${_CFG}    "" CACHE STRING "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_${_CFG} "" CACHE STRING "" FORCE)
    set(CMAKE_STATIC_LINKER_FLAGS_${_CFG} "" CACHE STRING "" FORCE)
    mark_as_advanced(
        CMAKE_CXX_FLAGS_${_CFG}
        CMAKE_EXE_LINKER_FLAGS_${_CFG}
        CMAKE_SHARED_LINKER_FLAGS_${_CFG}
        CMAKE_STATIC_LINKER_FLAGS_${_CFG}
    )
endforeach()

include(cmake/Profiles.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(src/document)
add_subdirectory(src/render)
add_subdirectory(src/terminal)
add_subdirectory(src/navigation)
add_subdirectory(src/viewer)
add_subdirectory(src/cache)
add_subdirectory(src/search)
add_subdirectory(src/app)

option(CPPDF_BUILD_TESTS "Build GTest unit tests" ON)
if(CPPDF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
