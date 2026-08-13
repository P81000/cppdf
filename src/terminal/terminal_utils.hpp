#pragma once

#include <sys/ioctl.h>
#include <unistd.h>
#include <expected>

#include "core/error.hpp"

namespace cppdf {
    struct TerminalInfo {
        unsigned short rows;
        unsigned short cols;
    };

    class Terminal {
        public:
            Terminal() = default;
            ~Terminal() = default;

            Terminal(const Terminal&) = delete;
            Terminal& operator=(const Terminal&) = delete;

            [[nodiscard]] std::expected<TerminalInfo, cppdf::Error> get_size() const;
    };
} // namespace cppdf
