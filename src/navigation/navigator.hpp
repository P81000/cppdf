#pragma once


#include "terminal/terminal_utils.hpp"
#include "app/state.hpp"
#include "core/types.hpp"

namespace cppdf {
    class Navigator {
        public:
            bool process_key(char key, Terminal& term, TerminalInfo& info, AppState& state);
        private:
            bool handle_command_mode(char key, AppState& state);
            bool handle_normal_mode(char key, Terminal& term, TerminalInfo& info, AppState& state);

            void execute_cmd(const std::string& cmd_str, AppState& state);
    };
} // namespace cppdf
