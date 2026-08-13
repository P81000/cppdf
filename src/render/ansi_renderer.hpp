#pragma once

#include "core/types.hpp"

namespace cppdf {
    class AnsiRenderer {
        public:
            void draw(const cppdf::Bitmap& bmp, size_t cols, size_t rows) const;
        private:
            // 2 escape codes ANSI RGB (\033[38;2;RRR;GGG;BBBm = 19 chars each)
            // + half-block char (3 bytes UTF-8) + end-of-line reset
            static constexpr size_t k_max_bytes_per_cell = 19 + 19 + 3; 

    };
} // namespace cppdf
