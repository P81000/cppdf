#pragma once

#include <expected>
#include <string_view>
#include <span>
#include <cstdint>

namespace cppdf {
    struct Bitmap {
        int width;
        int height;
        int stride;
        std::span<const uint8_t> pixels;
    };

    enum class DocumentError {
        FileNotFound,
        InvalidFormat,
        PasswordProtected,
        PageOutOfRange,
        InternalRenderError
    };

    template <typename T>
        concept PdfDocument = requires(T doc, std::string_view path, int page, float zoom) {
            { doc.open(path) } -> std::same_as<std::expected<void, DocumentError>>;
            { doc.get_page_count() } -> std::same_as<int>;
            { doc.render_page(page, zoom) } -> std::same_as<std::expected<Bitmap, DocumentError>>;
        };

} // namespace cppdf
