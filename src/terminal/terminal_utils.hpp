#pragma once

#include <sys/ioctl.h>
#include <unistd.h>
#include <expected>
#include <termios.h>

#include "core/error.hpp"
#include "core/types.hpp"

namespace cppdf {
    class Terminal {
        public:
            Terminal() = default;
            ~Terminal();

            Terminal(const Terminal&) = delete;
            Terminal& operator=(const Terminal&) = delete;

            [[nodiscard]] std::expected<TerminalInfo, Error> get_size() const;
            [[nodiscard]] std::expected<void, Error> enable_raw_mode();
            [[nodiscard]] char read_key() const;
            void disable_raw_mode();

        private:
            struct termios m_orig_termios{};
            bool m_raw_mode = false;
    };
} // namespace cppdf
