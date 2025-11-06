#include <string>
#include <vector>

#include "logger.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        log_error("PDF file not provided");
        log_warning("Usage: cppdf <path-to-pdf-file>.pdf");
        return 1;
    }

    std::string pdf_path = argv[1];
    log_debug("Trying to open %s", pdf_path.c_str());
    return 0;
}
