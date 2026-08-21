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

    struct TerminalInfo {
        unsigned short rows;
        unsigned short cols;
        unsigned short px_width;
        unsigned short px_height;
    };
} // namespace cppdf
