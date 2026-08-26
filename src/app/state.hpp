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
    bool running{true};
    bool needs_rasterize{true};
    bool needs_redraw{true};

    int current_page{0};
    int total_pages{0};
    float zoom_factor{cppdf::INITIAL_ZOOM};
    float dpi{cppdf::DEFAULT_DPI};
    size_t scroll_y{0uz};
    size_t max_scroll;
    std::optional<cppdf::Bitmap> current_bmp;

    AppMode mode{AppMode::Normal};
    std::string cmd_buff{""};
    std::string ui_message{""};
};

