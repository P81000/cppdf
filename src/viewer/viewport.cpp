#include <algorithm>

#include "viewport.hpp"

namespace cppdf {
    Layout calculate_layout(const TerminalInfo& info, const Bitmap& bmp, float zoom) {
        size_t target_cols = std::max(1uz, static_cast<size_t>(info.cols * zoom));
        size_t dest_col = (info.cols > target_cols) ? (info.cols - target_cols) / 2 + 1 : 1;

        float cell_w = static_cast<float>(info.px_width) / info.cols;
        float cell_h = static_cast<float>(info.px_height) / info.rows;

        float target_px_width = static_cast<float>(target_cols) * cell_w;
        float target_px_height = target_px_width * (static_cast<float>(bmp.height) / static_cast<float>(bmp.width));

        float precise_rows = target_px_height / cell_h;
        size_t total_rows = static_cast<size_t>(precise_rows + 0.5f);

        size_t final_target_rows = total_rows;
        size_t max_rows = info.rows - 1;
        size_t crop_h = bmp.height;
        size_t dest_row = 1;

        if (total_rows > max_rows) {
            final_target_rows = max_rows;
            crop_h = static_cast<size_t>(static_cast<float>(bmp.height) * (static_cast<float>(max_rows) / precise_rows));
        } else {
            dest_row = (max_rows - total_rows) / 2 + 1;
        }

        size_t max_scroll = (bmp.height > crop_h) ? (bmp.height - crop_h) : 0uz;

        return Layout {
            .target_cols = target_cols,
            .final_target_rows = final_target_rows,
            .dest_col = dest_col,
            .dest_row = dest_row,
            .crop_h = crop_h,
            .max_scroll = max_scroll

        };
    }
} // namespace cppdf
