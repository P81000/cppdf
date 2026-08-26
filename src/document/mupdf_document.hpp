#pragma once

#include <mupdf/fitz.h>
#include <memory>
#include <string>

#include "pdf_document.hpp"

namespace cppdf {
    struct FzContextDeleter {
        void operator()(fz_context* ctx) const noexcept {
            if (ctx) fz_drop_context(ctx);
        }
    };

    struct FzDocumentDeleter {
        fz_context* ctx = nullptr;

        void operator()(fz_document* doc) const noexcept {
            if (doc && ctx) fz_drop_document(ctx, doc);
        }
    };

    struct FzPixmapDeleter {
        fz_context* ctx = nullptr;

        void operator()(fz_pixmap* pix) const noexcept {
            if (pix && ctx) fz_drop_pixmap(ctx, pix);
        }
    };

    class MuPdfDocument {
        public:
            MuPdfDocument();
            ~MuPdfDocument() = default;

            MuPdfDocument(const MuPdfDocument&) = delete;
            MuPdfDocument& operator=(const MuPdfDocument&) = delete;

            [[nodiscard]] std::expected<void, Error> open(std::string_view path);
            [[nodiscard]] int get_page_count() const;
            [[nodiscard]] std::expected<Bitmap, Error> rasterize_page(int page_number, float dpi);

            void set_last_warning(const char* message);
            std::string get_last_warning() const { return std::string(last_error_msg); }
            void clear_warning() { last_error_msg = ""; }

        private:
            std::unique_ptr<fz_context, FzContextDeleter> m_ctx;
            std::unique_ptr<fz_document, FzDocumentDeleter> m_doc;
            std::unique_ptr<fz_pixmap, FzPixmapDeleter> m_pix;

            std::string last_error_msg{""};

            int page_count = 0;
    };

    static_assert(PdfDocument<MuPdfDocument>);

} // namespace cppdf
