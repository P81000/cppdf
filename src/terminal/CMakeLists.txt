#src/terminal/CMakeLists.txt

add_library(cppdf_terminal STATIC
    terminal_utils.cpp
)

target_include_directories(cppdf_terminal
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

cppdf_apply_profile(cppdf_terminal)
