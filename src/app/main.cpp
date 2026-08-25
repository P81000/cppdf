#include <string_view>
#include <print>

#include "document/mupdf_document.hpp"
#include "terminal/terminal_utils.hpp"
#include "render/kitty_renderer.hpp"
#include "config.hpp"

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
    std::optional<cppdf::Bitmap> current_bmp;

    AppMode mode{AppMode::Normal};
    std::string cmd_buff{""};
};

void rasterize_current_page(cppdf::MuPdfDocument& doc, cppdf::KittyRenderer& renderer, AppState& state) {
    auto bmp = doc.rasterize_page(state.current_page, state.dpi);
    if (bmp.has_value()) {
        state.current_bmp = std::move(bmp.value());
        renderer.upload(state.current_bmp.value());
    }
    state.needs_rasterize = false;
    state.needs_redraw = true;
}

void render_frame(const cppdf::TerminalInfo& info, const cppdf::KittyRenderer& renderer, AppState& state) {
    if (!state.current_bmp.has_value()) return;
    const auto& bmp = state.current_bmp.value();

    size_t target_cols = std::max(1uz, static_cast<size_t>(info.cols * state.zoom_factor));
    size_t dest_col = (info.cols > target_cols) ? (info.cols - target_cols) / 2 + 1 : 1;

    float cell_w = static_cast<float>(info.px_width) / info.cols;
    float cell_h = static_cast<float>(info.px_height) / info.rows;

    float target_px_width = static_cast<float>(target_cols) * cell_w;
    float target_px_height = target_px_width * (static_cast<float>(bmp.height) / static_cast<float>(bmp.width));

    float precise_rows = target_px_height / cell_h;
    size_t total_rows = static_cast<size_t>(precise_rows + 0.5f);

    size_t final_target_rows = total_rows;
    size_t dest_row = 1;
    size_t crop_h = bmp.height;

    if (total_rows > info.rows) {
        final_target_rows = info.rows;
        crop_h = static_cast<size_t>(static_cast<float>(bmp.height) * (static_cast<float>(info.rows) / precise_rows));
    } else {
        dest_row = (info.rows - total_rows) / 2 + 1;
    }

    renderer.place(target_cols, final_target_rows, dest_col, dest_row, crop_h, state.scroll_y);
    state.needs_redraw = false;
}

void handle_input(cppdf::Terminal& term, cppdf::TerminalInfo& info, AppState& state) {
    char key = term.read_key();
    switch(key) {
        case 'q':
            state.running = false;
            break;

        case 'r': {
                      auto new_size = term.get_size();
                      if (new_size.has_value()) {
                          info = new_size.value();
                          state.needs_redraw = true;
                      }
                      break;
                  }

        case 'j':
                  state.scroll_y += cppdf::SCROLL_STEP;
                  state.needs_redraw = true;
                  break;

        case 'k':
                  if (state.scroll_y > 100) state.scroll_y -= cppdf::SCROLL_STEP;
                  else state.scroll_y = 0;
                  state.needs_redraw = true;
                  break;

        case 'l':
        case 'n':
        case ' ':
                  if (state.current_page + 1 < state.total_pages) {
                      ++state.current_page;
                      state.scroll_y = 0;
                      state.needs_rasterize = true;
                  }
                  break;

        case 'h':
        case 'p':
                  if (state.current_page > 0) {
                      --state.current_page;
                      state.scroll_y = 0;
                      state.needs_rasterize = true;
                  }
                  break;

        case '+':
        case '=':
                  state.zoom_factor = std::min(cppdf::MAX_ZOOM, state.zoom_factor + cppdf::ZOOM_STEP);
                  state.needs_redraw = true;
                  break;

        case '-':
                  state.zoom_factor = std::max(cppdf::MIN_ZOOM, state.zoom_factor - cppdf::MIN_ZOOM);
                  state.needs_redraw = true;
                  break;

        case '0':
                  state.zoom_factor = cppdf::INITIAL_ZOOM;
                  state.scroll_y = 0;
                  state.needs_redraw = true;
                  break;

        default:
                  break;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::print(stderr, "Usage: cppdf <path_to_pdf>\n");
        return 1;
    }

    std::string_view pdf_path = argv[1];

    if (!pdf_path.ends_with(".pdf") && !pdf_path.ends_with(".PDF")) {
        std::print(stderr, "Error: File must be a .pdf document.\n");
        return 1;
    }

    cppdf::MuPdfDocument doc;
    cppdf::Terminal term;
    cppdf::KittyRenderer renderer;
    AppState state;

    auto result = doc.open(pdf_path);
    if (!result.has_value()) {
        std::print(stderr, "Error: Could not open document '{}'\n", pdf_path);
        return 1;
    }
    state.total_pages = doc.get_page_count();

    auto raw_enabled = term.enable_raw_mode();
    if (!raw_enabled.has_value()) {
        std::print(stderr, "Error: {}\n", cppdf::err_msg(raw_enabled.error()));
        return 1;
    }

    auto term_size = term.get_size();
    if (!term_size.has_value()) {
        std::print(stderr, "Error: {}\n", cppdf::err_msg(term_size.error()));
        return 1;
    }
    auto info = term_size.value();

    while (state.running) {
        if (state.needs_rasterize)
            rasterize_current_page(doc, renderer, state);

        if (state.needs_redraw && state.current_bmp.has_value())
            render_frame(info, renderer, state);

        handle_input(term, info, state);
    }

    renderer.clear();

    return 0;
}
