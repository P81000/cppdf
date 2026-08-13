#pragma once

#include <span>
#include <cstdint>

namespace cppdf {
    struct Bitmap {
        int width;
        int height;
        int stride;
        std::span<const uint8_t> pixels;
    };
} // namespace cppdf
