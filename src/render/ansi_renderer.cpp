#include <string>
#include <format>

#include "ansi_renderer.hpp"

namespace cppdf {
    void AnsiRenderer::draw(const cppdf::Bitmap& source, size_t cols, size_t rows) const {
        std::string buf;
        buf.reserve(cols * rows * k_max_bytes_per_cell);

        size_t bpp = static_cast<size_t>(source.stride / source.width);

        size_t actual_cols = static_cast<size_t>(source.width);
        size_t actual_rows = static_cast<size_t>(source.height) / 2;

        buf += "\033[H";

        for (auto y{0uz}; y < actual_rows; ++y) {
            for (auto x{0uz}; x < actual_cols; ++x) {
                size_t top_idx      = (y * 2) * static_cast<size_t>(source.stride) + (x * bpp);
                size_t bottom_idx   = (y * 2 + 1) * static_cast<size_t>(source.stride) + (x * bpp);

                uint8_t rt = source.pixels[top_idx + 0];
                uint8_t gt = source.pixels[top_idx + 1];
                uint8_t bt = source.pixels[top_idx + 2];

                uint8_t rb = source.pixels[bottom_idx + 0];
                uint8_t gb = source.pixels[bottom_idx + 1];
                uint8_t bb = source.pixels[bottom_idx + 2];

                std::format_to(std::back_inserter(buf),
                    "\033[38;2;{};{};{}m\033[48;2;{};{};{}m▀",
                    rt, gt, bt,   // foreground
                    rb, gb, bb    // background
                );
            }
            buf += "\033[0m\n";
        }

        std::fwrite(buf.data(), 1, buf.size(), stdout);
    }
} // namespace cppdf
