#include <string_view>
#include<print>

#include "document/mupdf_document.hpp"
#include "terminal/terminal_utils.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::print(stderr, "Usage: cppdf <path_to_pdf>\n");
        return 1;
    }

    std::string_view pdf_path = argv[1];

    cppdf::MuPdfDocument doc;

    auto result = doc.open(pdf_path);
    if (!result.has_value()) {
        std::print(stderr, "Error: Could not open document '{}'\n", pdf_path);
        return 1;
    }

    std::print(stdout, "Successfully opened PDF!\n");
    std::print(stdout, "Total pages: {}\n", doc.get_page_count());

    cppdf::Terminal term;
    auto term_size = term.get_size();
    if (!term_size.has_value()) {
        std::print(stderr, "Error: {}\n", term_size.error());
        return 1;
    }

    const auto& info = term_size.value();
    std::print(stdout, "{} {}\n", info.rows, info.cols);

    return 0;
}
