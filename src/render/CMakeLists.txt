#src/render/CMakeLists.txt

add_library(cppdf_render 
    kitty_renderer.cpp
)

target_include_directories(cppdf_render
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

cppdf_apply_profile(cppdf_render)
