#src/viewer/CMakeLists.txt

add_library(cppdf_viewer STATIC
    viewport.cpp
)

target_include_directories(cppdf_viewer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

cppdf_apply_profile(cppdf_viewer)
