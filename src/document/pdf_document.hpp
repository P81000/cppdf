#pragma once

#include <string_view>
#include <expected>

#include "core/types.hpp"
#include "core/error.hpp"

namespace cppdf {
    template <typename T>
        concept PdfDocument = requires(T doc, std::string_view path, int page_number, int target_w, int target_h) {
            { doc.open(path) } -> std::same_as<std::expected<void, cppdf::Error>>;
            { doc.get_page_count() } -> std::same_as<int>;
            { doc.rasterize_page(page_number, target_w, target_h) } -> std::same_as<std::expected<cppdf::Bitmap, cppdf::Error>>;
        };

} // namespace cppdf
