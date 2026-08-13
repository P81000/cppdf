#pragma once

#include <vector>
#include <cstdint>

#include "document/pdf_document.hpp"

namespace cppdf {
    class ImageScaler {
        public:
            virtual ~ImageScaler() = default;

            [[nodiscard]] virtual std::vector<uint8_t> scale(const Bitmap& source, size_t target_w, size_t target_h) const = 0;
    };

} // namespace cppdf
