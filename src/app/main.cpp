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
    bool needs_rasterize = true;
    bool needs_redraw = true;
    std::optional<cppdf::Bitmap> current_bmp;

    bool running = true;

    while (running) {
        if (needs_rasterize) {
            auto bmp = doc.rasterize_page(current_page, 200.0f);
            if (bmp.has_value()) {
                current_bmp = std::move(bmp.value());
            }
            needs_rasterize = false;
            needs_redraw = true;
        }

        if (needs_redraw && current_bmp.has_value()) {
            renderer.render(current_bmp.value(), info, scroll_y);
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
            default:
                break;
        }
    }

    renderer.clear();

    return 0;
}
