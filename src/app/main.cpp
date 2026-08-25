#include <string_view>
#include <print>

#include "document/mupdf_document.hpp"
#include "terminal/terminal_utils.hpp"
#include "render/kitty_renderer.hpp"

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

    auto result = doc.open(pdf_path);
    if (!result.has_value()) {
        std::print(stderr, "Error: Could not open document '{}'\n", pdf_path);
        return 1;
    }

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

    int current_page = 0;
    int total_pages = doc.get_page_count();

    auto scroll_y{0uz};
    float zoom_factor{1.0f};
    bool needs_rasterize = true;
    bool needs_redraw = true;
    std::optional<cppdf::Bitmap> current_bmp;

    bool running = true;

    while (running) {
        if (needs_rasterize) {
            auto bmp = doc.rasterize_page(current_page, 200.0f);
            if (bmp.has_value()) {
                current_bmp = std::move(bmp.value());
                renderer.upload(current_bmp.value());
            }
            needs_rasterize = false;
            needs_redraw = true;
        }

        if (needs_redraw && current_bmp.has_value()) {
            const auto& bmp = current_bmp.value();

            size_t target_cols = std::max(1uz, static_cast<size_t>(info.cols * zoom_factor));
            size_t dest_col = (info.cols > target_cols) ? (info.cols - target_cols) / 2 + 1 : 1;

            float cell_w = static_cast<float>(info.px_width) / info.cols;
            float cell_h = static_cast<float>(info.px_height) / info.rows;

            float target_px_width = static_cast<float>(target_cols) * cell_w;
            float target_px_height = target_px_width * (static_cast<float>(bmp.height) / static_cast<float>(bmp.width));

            float precise_rows = target_px_height / cell_h;
            size_t total_rows = static_cast<size_t>(precise_rows + 0.5f);

            // 3. Float or crop
            size_t final_target_rows = total_rows;
            size_t dest_row = 1;
            size_t crop_h = bmp.height;

            if (total_rows > info.rows) {
                // Overflow: Fix to top, use all rows, crop source image
                final_target_rows = info.rows;
                crop_h = static_cast<size_t>(static_cast<float>(bmp.height) * (static_cast<float>(info.rows) / precise_rows));
            } else {
                // Fit: No crop, center vertically (White Sheet float)
                dest_row = (info.rows - total_rows) / 2 + 1;
            }

            renderer.place(target_cols, final_target_rows, dest_col, dest_row, crop_h, scroll_y);
            needs_redraw = false;
        }

        char key = term.read_key();
        switch(key) {
            case 'q':
                running = false;
                break;
            case 'r': {
                auto new_size = term.get_size();
                if (new_size.has_value()) {
                    info = new_size.value();
                    needs_redraw = true;
                }
                break;
            }
            case 'j':
                scroll_y += 100;
                needs_redraw = true;
                break;
            case 'k':
                if (scroll_y > 100) scroll_y -= 100;
                else scroll_y = 0;
                needs_redraw = true;
                break;
            case 'l':
            case 'n':
            case ' ':
                if (current_page + 1 < total_pages) {
                    ++current_page;
                    scroll_y = 0;
                    needs_rasterize = true;
                }
                break;
            case 'h':
            case 'p':
                if (current_page > 0) {
                    --current_page;
                    scroll_y = 0;
                    needs_rasterize = true;
                }
                break;
            case '+':
            case '=':
                zoom_factor = std::min(1.0f, zoom_factor + 0.10f);
                needs_redraw = true;
                break;
            case '-':
                zoom_factor = std::max(0.10f, zoom_factor - 0.10f);
                needs_redraw = true;
                break;
            case '0':
                zoom_factor = 1.0f;
                scroll_y = 0;
                needs_redraw = true;
                break;
            default:
                break;
        }
    }

    renderer.clear();

    return 0;
}
