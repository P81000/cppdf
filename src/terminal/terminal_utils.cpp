#include <cstdio>

#include "terminal_utils.hpp"

namespace cppdf {
    Terminal::~Terminal() {
        disable_raw_mode();
    }

    std::expected<TerminalInfo, Error> Terminal::get_size() const {
        struct winsize ws{};

        if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
            return std::unexpected(Error::TerminalQueryFailed);
        }

        return TerminalInfo{ ws.ws_row, ws.ws_col, ws.ws_xpixel, ws.ws_ypixel };
    }

    std::expected<void, Error> Terminal::enable_raw_mode() {
        if (m_raw_mode) return {};

        if (::tcgetattr(STDIN_FILENO, &m_orig_termios) == -1) {
            return std::unexpected(Error::TerminalQueryFailed);
        }

        struct termios raw = m_orig_termios;
        raw.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON));

        if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
            return std::unexpected(Error::TerminalQueryFailed);
        }

        m_raw_mode = true;

        std::fwrite("\033[?1049h\033[?25l", 1, 14, stdout);
        std::fflush(stdout);

        return {};
    }

    void Terminal::disable_raw_mode() {
        if (!m_raw_mode) return;

        ::tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_orig_termios);
        m_raw_mode = false;

        std::fwrite("\033[?1049l\033[?25h", 1, 14, stdout);
        std::fflush(stdout);
    }

    char Terminal::read_key() const {
        char c = '\0';
        if (::read(STDIN_FILENO, &c, 1) <= 0) {
            return '\0';
        }
        return c;
    }
} // namespace cppdf
