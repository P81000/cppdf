#src/navigation/CMakeLists.txt

add_library(cppdf_navigation STATIC
    navigator.cpp
)

target_include_directories(cppdf_navigation
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

cppdf_apply_profile(cppdf_navigation)
