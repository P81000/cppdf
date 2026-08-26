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

        size_t i = 0uz;
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

    void KittyRenderer::clear() {
        std::fwrite("\033_Ga=d,d=A,q=2\033\\\033[2J\033[H", 1, 23, stdout);
    }

    void KittyRenderer::upload(const Bitmap& bmp) {
        if (bmp.pixels.empty() || bmp.height == 0 || bmp.width == 0) return;

        if (m_image_id != 0) {
            auto del = std::format("\033_Ga=d,d=I,i={},q=2\033\\", m_image_id);
            std::fwrite(del.data(), 1, del.size(), stdout);
        }

        m_image_id      = 1;
        m_src_width     = bmp.width;
        m_src_height    = bmp.height;

        std::string payload = base64_encode(bmp.pixels);
        static constexpr size_t k_chunk_size = 4096uz;
        size_t offset = 0uz;
        bool is_first = true;

        while (offset < payload.size()) {
            auto remaining = payload.size() - offset;
            auto len = std::min(remaining, k_chunk_size);
            std::string_view chunk(payload.data() + offset, len);
            offset += len;

            bool has_more = (offset < payload.size());

            if (is_first) {
                auto header = std::format("\033_Ga=t,f={},q=2,s={},v={},i={},m={};",
                        k_rgb32_format, bmp.width, bmp.height, m_image_id, (has_more ? 1 : 0));

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

        std::fflush(stdout);
    }

    void KittyRenderer::place(size_t target_cols, size_t target_rows, size_t dest_col, size_t dest_row, size_t crop_h, size_t scroll_y) const {
        if (m_image_id == 0) return;

        auto max_scroll = (m_src_height > crop_h) ? (m_src_height - crop_h) : 0uz;
        auto safe_scroll_y = std::min(scroll_y, max_scroll);

        std::fwrite("\033[?2026h", 1, 8, stdout);

        std::fwrite("\033_Ga=d,d=a,q=2\033\\", 1, 16, stdout);

        auto cursor_seq = std::format("\033[{};{}H", dest_row, dest_col);
        std::fwrite(cursor_seq.data(), 1, cursor_seq.size(), stdout);

        auto cmd = std::format("\033_Ga=p,q=2,i={},c={},r={},x=0,y={},w={},h={}\033\\", m_image_id, target_cols, target_rows, safe_scroll_y, m_src_width, crop_h);

        std::fwrite(cmd.data(), 1, cmd.size(), stdout);

        std::fwrite("\033[?2026l", 1, 8, stdout);
        std::fflush(stdout);
    }
} // namespace cppdf
