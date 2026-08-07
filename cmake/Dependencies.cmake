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
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(MUPDF QUIET mupdf)
endif()

if(NOT MUPDF_FOUND)
    find_path(MUPDF_INCLUDE_DIRS mupdf/fitz.h
        PATHS "$ENV{HOME}/dev/.homebrew/include"
    )
    find_library(MUPDF_LINK_LIBRARIES mupdf
        PATHS "$ENV{HOME}/dev/.homebrew/lib"
    )
    if(MUPDF_INCLUDE_DIRS AND MUPDF_LINK_LIBRARIES)
        set(MUPDF_FOUND TRUE)
    endif()
endif()

if(MUPDF_FOUND)
    message(STATUS "MuPDF found: ${MUPDF_LINK_LIBRARIES}")
    if(NOT TARGET mupdf::mupdf)
        add_library(mupdf::mupdf UNKNOWN IMPORTED)
        set_target_properties(mupdf::mupdf PROPERTIES
            IMPORTED_LOCATION             "${MUPDF_LINK_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${MUPDF_INCLUDE_DIRS}"
        )
    endif()
else()
    message(WARNING "MuPDF not found — check your Homebrew installation")
endif()
