#include <print>

#include "document/mupdf_document.hpp"
#include "terminal/terminal_utils.hpp"
#include "render/kitty_renderer.hpp"
#include "navigation/navigator.hpp"
#include "viewer/viewport.hpp"
#include "state.hpp"

void rasterize_current_page(cppdf::MuPdfDocument& doc, cppdf::KittyRenderer& renderer, AppState& state) {
    auto bmp = doc.rasterize_page(state.current_page, state.dpi);
    std::string warning = doc.get_last_warning();

    if (!warning.empty()) {
        state.ui_message = warning;
        doc.clear_warning();
    }

    if (bmp.has_value()) {
        state.current_bmp = std::move(bmp.value());
        renderer.upload(state.current_bmp.value());
    }
    state.needs_rasterize = false;
    state.needs_redraw = true;
}

void render_status_bar(const cppdf::TerminalInfo& info, const AppState& state) {
    auto cursor_seq = std::format("\033[{};1H", info.rows);
    std::fwrite(cursor_seq.data(), 1, cursor_seq.size(), stdout);

    std::string left_str;
    if (!state.ui_message.empty())
        left_str = std::format(" WARNING: {} ", state.ui_message);
    else
        left_str = (state.mode == AppMode::Normal) ? " NORMAL " : std::format(":{}", state.cmd_buff);

    std::string right_str = std::format(" page {}/{} | zoom: {:.0f}% | scroll: {}px | {} ",
                                    state.current_page + 1,
                                    state.total_pages,
                                    state.zoom_factor * 100.0f,
                                    state.scroll_y,
                                    (state.mode == AppMode::Normal) ? state.cmd_buff : " ");

    size_t spaces = info.cols - left_str.size() - right_str.size();
    if (spaces < 0) spaces = 0;

    auto status_text = left_str + std::string(spaces, ' ') + right_str;

    auto final_bar = std::format("\033[7m{}\033[m", status_text);

    std::fwrite(final_bar.data(), 1, final_bar.size(), stdout);
    std::fflush(stdout);
}

void render_frame(const cppdf::TerminalInfo& info, const cppdf::KittyRenderer& renderer, AppState& state) {
    if (!state.current_bmp.has_value()) return;

    auto layout = cppdf::calculate_layout(info, state.current_bmp.value(), state.zoom_factor);

    state.max_scroll = layout.max_scroll;
    state.scroll_y = std::min(state.scroll_y, layout.max_scroll);

    renderer.place(layout.target_cols, layout.final_target_rows, layout.dest_col, layout.dest_row, layout.crop_h, state.scroll_y);
    render_status_bar(info, state);
    state.needs_redraw = false;
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
    cppdf::Navigator navigator;
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

        char key = term.read_key();
        if (!navigator.process_key(key, term, info, state))
            state.running = false;
    }

    renderer.clear();

    return 0;
}
