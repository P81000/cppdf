# tests/CMakeLists.txt

include(GoogleTest)

add_executable(cppdf_tests
    mupdf_document_test.cpp
)

target_link_libraries(cppdf_tests
    PRIVATE
        cppdf_document
        GTest::gtest_main
)

cppdf_apply_profile(cppdf_tests)

gtest_discover_tests(cppdf_tests)
