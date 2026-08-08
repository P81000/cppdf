#include <new>
#include <string>

#include "mupdf_document.hpp"

namespace cppdf {
    MuPdfDocument::MuPdfDocument() {
        m_ctx.reset(fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED));
        if (!m_ctx) {
            throw std::bad_alloc();
        }

        fz_register_document_handlers(m_ctx.get());
    }

    std::expected<void, DocumentError> MuPdfDocument::open(std::string_view path) {
        bool rc = false;

        fz_try(m_ctx.get()) {
            fz_document* raw_doc = fz_open_document(m_ctx.get(), std::string(path).c_str());

            m_doc = std::unique_ptr<fz_document, FzDocumentDeleter>(raw_doc, FzDocumentDeleter{m_ctx.get()});

            page_count = fz_count_pages(m_ctx.get(), m_doc.get());

            rc = true;

        } fz_catch(m_ctx.get()) {
            rc = false;
        }

        if (rc == false) return std::unexpected(DocumentError::FileNotFound);

        return {};
    }

    int MuPdfDocument::get_page_count() const {
        return page_count;
    }

    std::expected<Bitmap, DocumentError> MuPdfDocument::render_page(int page_number, float zoom) {
        if (page_number < 0 || page_number >= page_count) {
            return std::unexpected(DocumentError::PageOutOfRange);
        }

        bool rc = false;
        int width   = 0;
        int height  = 0;
        int stride  = 0;
        std::span<const uint8_t> pixels;

        fz_try(m_ctx.get()) {
            fz_page *page = fz_load_page(m_ctx.get(), m_doc.get(), page_number);
            fz_matrix ctm = fz_scale(zoom / 100.0f, zoom / 100.0f);
            fz_pixmap *pixmap = fz_new_pixmap_from_page(m_ctx.get(), page, ctm, fz_device_rgb(m_ctx.get()), 1);

            fz_drop_page(m_ctx.get(), page);

            m_pix = std::unique_ptr<fz_pixmap, FzPixmapDeleter>(pixmap, FzPixmapDeleter{m_ctx.get()});

            width   = fz_pixmap_width(m_ctx.get(), m_pix.get());
            height  = fz_pixmap_height(m_ctx.get(), m_pix.get());
            stride  = fz_pixmap_stride(m_ctx.get(), m_pix.get());

            pixels = {
                fz_pixmap_samples(m_ctx.get(), m_pix.get()),
                static_cast<size_t>(stride * height)
            };

            rc = true;
        } fz_catch(m_ctx.get()) {
            rc = false;
        }

        if (rc == false) return std::unexpected(DocumentError::InternalRenderError);

        return Bitmap { width, height, stride, pixels };
    }
} // namespace cppdf
