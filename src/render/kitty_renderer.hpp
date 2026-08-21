#pragma once

#include <string>
#include <span>

#include "core/types.hpp"

namespace cppdf {
    class KittyRenderer {
        public:
            void render(const Bitmap& bmp, const TerminalInfo& t_info, size_t scroll_y) const;

            void clear() const;
        private:
            static std::string base64_encode(std::span<const uint8_t> data);
    };
} // namespace cppdf
