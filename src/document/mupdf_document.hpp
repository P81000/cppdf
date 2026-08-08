#pragma once

#include <mupdf/fitz.h>
#include <memory>

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

            std::expected<void, DocumentError> open(std::string_view path);
            int get_page_count() const;
            std::expected<Bitmap, DocumentError> render_page(int page, float zoom = 100.0f);

        private:
            std::unique_ptr<fz_context, FzContextDeleter> m_ctx;
            std::unique_ptr<fz_document, FzDocumentDeleter> m_doc;
            std::unique_ptr<fz_pixmap, FzPixmapDeleter> m_pix;

            int page_count = 0;
    };

    static_assert(PdfDocument<MuPdfDocument>);

} // namespace cppdf
