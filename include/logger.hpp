#pragma once

#include <string_view>
#include <string>

enum class LogLevel {
    LOG_DEBUG,
    LOG_WARN,
    LOG_ERROR
};

void log(LogLevel level, std::string_view msg, ...);

void log_debug(std::string_view msg, ...);
void log_warning(std::string_view msg, ...);
void log_error(std::string_view msg, ...);
