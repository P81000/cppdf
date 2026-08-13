#include "terminal_utils.hpp"

namespace cppdf {
    std::expected<TerminalInfo, cppdf::Error> Terminal::get_size() const {
        struct winsize ws{};

        if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
            return std::unexpected(cppdf::Error::TerminalQueryFailed);
        }

        return TerminalInfo{ ws.ws_row, ws.ws_col };
    }

} // namespace cppdf
