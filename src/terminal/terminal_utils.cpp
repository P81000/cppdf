#include "terminal_utils.hpp"

namespace cppdf {
    std::expected<TerminalInfo, std::string> Terminal::get_size() const {
        struct winsize ws{};

        if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
            return std::unexpected("[ ERROR ] Failed to get terminal size");
        }

        return TerminalInfo{ ws.ws_row, ws.ws_col };
    }

} // namespace cppdf
