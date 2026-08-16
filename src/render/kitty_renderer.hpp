#pragma once

#include <string>
#include <span>

#include "core/types.hpp"

namespace cppdf {
    class KittyRenderer {
        public:
            void render(const Bitmap& bmp, size_t target_cols, size_t target_rows) const;

            void clear() const;
        private:
            static std::string base64_encode(std::span<const uint8_t> data);
    };
} // namespace cppdf
