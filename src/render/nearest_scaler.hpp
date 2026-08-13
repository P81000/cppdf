#pragma once

#include "scaler.hpp"

namespace cppdf {
    class NearestNeighborScaler : public ImageScaler {
        public:
            [[nodiscard]] std::vector<uint8_t> scale(const Bitmap& source, size_t target_w, size_t target_h) const override;
    };
} // namespace cppdf
