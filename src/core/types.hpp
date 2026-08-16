#pragma once

#include <span>
#include <cstdint>

namespace cppdf {
    struct Bitmap {
        size_t width;
        size_t height;
        size_t stride;
        std::span<const uint8_t> pixels;
    };
} // namespace cppdf
