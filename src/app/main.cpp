#include <iostream>
#include <string_view>
#include "document/mupdf_document.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: cppdf <path_to_pdf>\n";
        return 1;
    }

    std::string_view pdf_path = argv[1];

    cppdf::MuPdfDocument doc;

    auto result = doc.open(pdf_path);
    if (!result.has_value()) {
        std::cerr << "Error: Could not open document '" << pdf_path << "'\n";
        return 1;
    }

    std::cout << "Successfully opened PDF!\n";
    std::cout << "Total pages: " << doc.get_page_count() << '\n';

    return 0;
}
