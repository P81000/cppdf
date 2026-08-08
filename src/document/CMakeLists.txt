# src/document/CMakeLists.txt

add_library(cppdf_document STATIC
    mupdf_document.cpp
)

target_include_directories(cppdf_document
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(cppdf_document
    PUBLIC
        mupdf::mupdf
)

cppdf_apply_profile(cppdf_document)
