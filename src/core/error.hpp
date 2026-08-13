#pragma once

#include <cstdint>
#include <string_view>

namespace cppdf {
    enum class Error : uint8_t {
        // Document
        FileNotFound,
        InvalidFormat,
        PasswordProtected,
        PageOutOfRange,
        InternalRenderError,

        // Terminal
        TerminalQueryFailed,

        // Generic
        Unknown
    };

    [[nodiscard]] inline constexpr std::string_view err_msg(Error e) noexcept {
        switch (e) {
            case cppdf::Error::FileNotFound:            return "File not found";
            case cppdf::Error::InvalidFormat:           return "Invalid Format";
            case cppdf::Error::PasswordProtected:       return "Password protected file";
            case cppdf::Error::PageOutOfRange:          return "Page out of range";
            case cppdf::Error::InternalRenderError:     return "Render error";
            case cppdf::Error::TerminalQueryFailed:     return "Failed to get terminal size";
            default: return "Unknown error";
        }
    };
} // namespace cppdf
