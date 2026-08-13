#include <gtest/gtest.h>
#include "document/mupdf_document.hpp"

using namespace cppdf;

TEST(MuPdfDocumentTest, HandlesInvalidFileGracefully) {
    MuPdfDocument doc;

    auto result = doc.open("/path/to/absolute/nowhere/ghost.pdf");

    EXPECT_FALSE(result.has_value());

    EXPECT_EQ(result.error(), Error::FileNotFound);
}

TEST(MuPdfDocumentTest, PageCountIsZeroWhenEmpty) {
    MuPdfDocument doc;

    EXPECT_EQ(doc.get_page_count(), 0);
}
