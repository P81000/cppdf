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
    const auto& info = term_size.value();

    int current_page = 0;
    int total_pages = doc.get_page_count();

    bool running = true;
    bool dirty = true;

    while (running) {
        if (dirty) {
            auto bmp = doc.rasterize_page(current_page, 150.0f);
            if (bmp.has_value()) {
                renderer.render(bmp.value(), info.cols, info.rows);
            }
            dirty = false;
        }

        char key = term.read_key();
        switch (key) {
            case 'q':
                running = false;
                break;
            case 'n':
            case 'j':
            case 'l':
                if (current_page + 1 < total_pages) {
                    ++current_page;
                    dirty = true;
                }
                break;
            case 'p':
            case 'k':
            case 'h':
                if (current_page > 0) {
                    --current_page;
                    dirty = true;
                }
                break;
            default:
                break;
        }
    }
    renderer.clear();

    return 0;
}
