add_executable(cppdf main.cpp)

target_link_libraries(cppdf
    PRIVATE
        cppdf_document
        cppdf_render
        cppdf_terminal
        cppdf_navigation
        cppdf_viewer
        cppdf_cache
        cppdf_search
)

cppdf_apply_profile(cppdf)
