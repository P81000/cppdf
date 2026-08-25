#pragma once

#include <cstddef>

#include "core/types.hpp"
#include "terminal/terminal_utils.hpp"

namespace cppdf {
    struct Layout {
        size_t target_cols;
        size_t final_target_rows;
        size_t dest_col;
        size_t dest_row;
        size_t crop_h;
        size_t max_scroll;
    };

    Layout calculate_layout(const TerminalInfo& info, const Bitmap& bmp, float zoom);
} // namespace cppdf
