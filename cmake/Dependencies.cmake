include(FetchContent)

# ─── GTest ────────────────────────────────────────────────────────────────────
FetchContent_Declare(
  googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ─── MuPDF ────────────────────────────────────────────────────────────────────
find_package(PkgConfig REQUIRED)
pkg_check_modules(MUPDF REQUIRED IMPORTED_TARGET mupdf)

if(NOT TARGET mupdf::mupdf)
    add_library(mupdf::mupdf ALIAS PkgConfig::MUPDF)
endif()
