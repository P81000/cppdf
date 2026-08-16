#pragma once

#include <string_view>
#include <expected>

#include "core/types.hpp"
#include "core/error.hpp"

namespace cppdf {
    template <typename T>
        concept PdfDocument = requires(T doc, std::string_view path, int page_number, float dpi) {
            { doc.open(path) } -> std::same_as<std::expected<void, Error>>;
            { doc.get_page_count() } -> std::same_as<int>;
            { doc.rasterize_page(page_number, dpi) } -> std::same_as<std::expected<Bitmap, Error>>;
        };

} // namespace cppdf
