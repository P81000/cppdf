#include <charconv>
#include <string>

#include "navigator.hpp"

namespace cppdf {
    bool Navigator::handle_normal_mode(char key, Terminal& term, TerminalInfo& info, AppState& state) {
        if (!state.cmd_buff.empty()) {
            if (state.cmd_buff == "g" && key == 'g') {
                state.scroll_y = 0;
                state.needs_redraw = true;
            }

            state.cmd_buff.clear();
            return true;
        }

        switch(key) {
            case 'q': // quit
                return false;

            case ':':
                state.mode = AppMode::Command;
                state.cmd_buff.clear();
                state.needs_redraw = true;
                break;

            case 'r':  { // redraw
                auto new_size = term.get_size();
                if (new_size.has_value()) {
                    info = new_size.value();
                    state.needs_redraw = true;
                }
                break;
            }

            case 'j': // scroll down
                state.scroll_y += SCROLL_STEP;
                state.needs_redraw = true;
                break;

            case 'k': // scroll up
                if (state.scroll_y > SCROLL_STEP) state.scroll_y -= SCROLL_STEP;
                else state.scroll_y = 0;
                state.needs_redraw = true;
                break;

            case 'l': case 'n': case ' ': // next page
                if (state.current_page + 1 < state.total_pages) {
                    ++state.current_page;
                    state.scroll_y = 0;
                    state.needs_rasterize = true;
                }
                break;

            case 'h': case 'p': // previous page
                if (state.current_page > 0) {
                    --state.current_page;
                    state.scroll_y = 0;
                    state.needs_rasterize = true;
                }
                break;

            case '+': case '=': // zoom in
                state.zoom_factor = std::min(MAX_ZOOM, state.zoom_factor + ZOOM_STEP);
                state.needs_redraw = true;
                break;

            case '-': // zoom out
                state.zoom_factor = std::max(MIN_ZOOM, state.zoom_factor - ZOOM_STEP);
                state.needs_redraw = true;
                break;

            case '0': // restore zoom
                state.zoom_factor = INITIAL_ZOOM;
                state.needs_redraw = true;
                break;

            case 'g': // gg - full scroll to top
                state.cmd_buff = "g";
                state.needs_redraw = true;
                break;

            case 'G': // G - full scroll to bottom
                state.scroll_y = SIZE_MAX;
                state.needs_redraw = true;
                break;
        }

        return true;
    }


    void Navigator::execute_cmd(const std::string& cmd_str, AppState& state) {
        if (cmd_str.empty()) return;

        if (cmd_str == "q" || cmd_str == "quit") {
            state.running = false;
            return;
        }

        int page_number = 0;
        auto [ptr, ec] = std::from_chars(cmd_str.data(), cmd_str.data() + cmd_str.size(), page_number);

        if (ec == std::errc() && ptr == cmd_str.data() + cmd_str.size()) {
            int target_idx = page_number - 1;

            if (target_idx < 0) target_idx = 0;
            else if (target_idx >= state.total_pages) target_idx = state.total_pages - 1;

            if (state.current_page != target_idx) {
                state.current_page = target_idx;
                state.scroll_y = 0;
                state.needs_rasterize = true;
            }
        }
    }

    bool Navigator::handle_command_mode(char key, AppState& state) {
        if (key == '\r' || key == '\n') {
            execute_cmd(state.cmd_buff, state);

            state.cmd_buff.clear();
            state.mode = AppMode::Normal;
            state.needs_redraw = true;
            return true;
        }

        if (key == 127 || key == '\b') {
            if (!state.cmd_buff.empty()) state.cmd_buff.pop_back();

            if (state.cmd_buff.empty()) state.mode = AppMode::Normal;

            state.needs_redraw = true;
            return true;
        }

        if (std::isprint(static_cast<unsigned char>(key))) {
            state.cmd_buff += key;
            state.needs_redraw = true;
        }
        return true;
    }

    bool Navigator::process_key(char key, Terminal& term, TerminalInfo& info, AppState& state) {
        if (!state.ui_message.empty()) {
            state.ui_message.clear();
            state.needs_redraw = true;
        }

        if (key == 27) { // ESC
            state.mode = AppMode::Normal;
            state.cmd_buff.clear();
            state.needs_redraw = true;
            return true;
        }

        if (state.mode == AppMode::Command) {
            return handle_command_mode(key, state);
        } else if (state.mode == AppMode::Normal) {
            return handle_normal_mode(key, term, info, state);
        }

        return true;
    }
} // namespace cppdf
