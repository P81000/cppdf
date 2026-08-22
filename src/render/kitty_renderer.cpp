#include <cstdio>
#include <format>
#include <string_view>

#include "kitty_renderer.hpp"

namespace cppdf {
    std::string KittyRenderer::base64_encode(std::span<const uint8_t> data) {
        static const char k_table[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string encoded;
        encoded.reserve(((data.size() + 2) / 3) * 4);

        auto i{0uz};
        while (i + 2 < data.size()) {
            uint32_t octet = static_cast<uint32_t>((data[i] << 16) | (data[i + 1] << 8) | data[i + 2]);

            encoded += k_table[(octet >> 18) & 0x3F];
            encoded += k_table[(octet >> 12) & 0x3F];
            encoded += k_table[(octet >> 6) & 0x3F];
            encoded += k_table[octet & 0x3F];

            i += 3;
        }

        if (i < data.size()) {
            if (data.size() - i == 1) {
                uint32_t octet = static_cast<uint32_t>((data[i] << 16));
                encoded += k_table[(octet >> 18) & 0x3F];
                encoded += k_table[(octet >> 12) & 0x3F];
                encoded += "==";
            } else {
                uint32_t octet = static_cast<uint32_t>((data[i] << 16) | (data[i + 1] << 8));
                encoded += k_table[(octet >> 18) & 0x3F];
                encoded += k_table[(octet >> 12) & 0x3F];
                encoded += k_table[(octet >> 6) & 0x3F];
                encoded += "=";
            }
        }

        return encoded;
    }

    void KittyRenderer::clear() const {
        std::fwrite("\033_Ga=d,d=A\033\\\033[2J\033[H", 1, 19, stdout);
    }

    void KittyRenderer::render(const Bitmap& bmp, size_t target_cols, size_t target_rows, size_t dest_col, size_t dest_row, size_t crop_h, size_t scroll_y) const {
        if (bmp.pixels.empty() || bmp.width == 0 || bmp.height == 0) return;

        std::fwrite("\033[?2026h", 1, 8, stdout);

        std::string payload = base64_encode(bmp.pixels);
        clear();

        auto cursor_seq = std::format("\033[{};{}H", dest_row, dest_col);
        std::fwrite(cursor_seq.data(), 1, cursor_seq.size(), stdout);

        auto max_scroll = (bmp.height > crop_h) ? (bmp.height - crop_h) : 0;
        auto safe_scroll_y = std::min(scroll_y, max_scroll);

        static constexpr auto k_chunk_size{4096uz};
        auto offset{0uz};
        bool is_first = true;

        while (offset < payload.size()) {
            auto remaining = payload.size() - offset;
            auto len = std::min(remaining, k_chunk_size);
            std::string_view chunk(payload.data() + offset, len);
            offset += len;

            bool has_more = (offset < payload.size());

            if (is_first) {
                auto header = std::format("\033_Ga=T,f=32,s={},v={},c={},r={},x=0,y={},w={},h={},m={};",
                    bmp.width, bmp.height, target_cols, target_rows, safe_scroll_y, bmp.width, crop_h, (has_more ? 1 : 0)
                );

                std::fwrite(header.data(), 1, header.size(), stdout);
                std::fwrite(chunk.data(), 1, chunk.size(), stdout);
                std::fwrite("\033\\", 1, 2, stdout);
                is_first = false;
            } else {
                auto header = std::format("\033_Gm={};", (has_more ? 1 : 0));
                std::fwrite(header.data(), 1, header.size(), stdout);
                std::fwrite(chunk.data(), 1, chunk.size(), stdout);
                std::fwrite("\033\\", 1, 2, stdout);
            }
        }

        std::fwrite("\033[?2026l", 1, 8, stdout);
        std::fflush(stdout);
    }
} // namespace cppdf
