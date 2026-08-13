#include "nearest_scaler.hpp"

namespace cppdf {
    std::vector<uint8_t> NearestNeighborScaler::scale(const Bitmap& source, size_t target_w, size_t target_h) const {

        size_t bpp = static_cast<size_t>(source.stride / source.width);
        std::vector<uint8_t> dest(target_w * target_h * bpp);

        for (auto y{0uz}; y < target_h; ++y) {
            for (auto x{0uz}; x < target_w; ++x) {
                size_t src_x = (x * static_cast<size_t>(source.width)) / target_w;
                size_t src_y = (y * static_cast<size_t>(source.height)) / target_h;

                size_t src_index = (src_y * static_cast<size_t>(source.width) + src_x) * bpp;
                size_t dest_index = (y * target_w + x) * bpp;

                for (auto i{0uz}; i < bpp; ++i) {
                    dest[dest_index + i] = source.pixels[src_index + i];
                }
            }
        }
        return dest;
    }

} // namespace cppdf
