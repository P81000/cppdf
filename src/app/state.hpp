#pragma once

#include <optional>
#include <string>

#include "config.hpp"
#include "core/types.hpp"

enum class AppMode {
    Normal,
    Command,
    Search // TODO: TO IMPLEMENT LATER
};

struct AppState {
    bool needs_rasterize    = true;
    bool needs_redraw       = true;
    bool running            = true;

    std::optional<cppdf::Bitmap> current_bmp;
    float dpi           = cppdf::DEFAULT_DPI;
    float zoom_factor   = cppdf::INITIAL_ZOOM;
    int current_page    = 0;
    int total_pages     = 0;
    size_t scroll_y     = 0uz;
    size_t max_scroll;

    AppMode mode            = AppMode::Normal;
    std::string cmd_buff    = "";
    std::string ui_message  = "";
};

