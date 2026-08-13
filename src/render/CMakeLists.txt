add_library(cppdf_render 
    ansi_renderer.cpp
    nearest_scaler.cpp
)

target_include_directories(cppdf_render
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

cppdf_apply_profile(cppdf_render)
