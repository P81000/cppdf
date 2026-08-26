#pragma once

#include <string>
#include <span>

#include "core/types.hpp"

namespace cppdf {
    class KittyRenderer {
        public:
            void place(size_t target_cols, size_t target_rows, size_t dest_col, size_t dest_row, size_t crop_h, size_t scroll_y) const;

            void upload(const Bitmap& bmp);

            void clear();
        private:
            static std::string base64_encode(std::span<const uint8_t> data);

            static constexpr uint32_t k_rgb32_format = 32;

            uint32_t m_image_id = 0;
            size_t m_src_height = 0;
            size_t m_src_width = 0;

    };
} // namespace cppdf
