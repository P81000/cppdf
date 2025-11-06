#pragma once

enum class LogLevel {
    LOG_DEBUG,
    LOG_WARN,
    LOG_ERROR
};

void log(LogLevel level, const char *msg, ...);

void log_debug(const char *msg, ...);
void log_warning(const char *msg, ...);
void log_error(const char *msg, ...);
