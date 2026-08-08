include(FetchContent)

# ─── GTest ────────────────────────────────────────────────────────────────────
FetchContent_Declare(
  googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ─── MuPDF (vcpkg) ────────────────────────────────────────────────────────────
find_package(unofficial-libmupdf CONFIG REQUIRED)

if(NOT TARGET mupdf::mupdf)
  # E aqui também usamos o namespace do vcpkg
  add_library(mupdf::mupdf ALIAS unofficial::libmupdf::libmupdf)
endif()
