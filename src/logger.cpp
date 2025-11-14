#include "logger.hpp"
#include <cstdarg>
#include <cstdio>

static void v_log(LogLevel level, std::string_view msg, va_list args) {
    const char *prefix = "";
    FILE *stream = stdout;

    switch (level) {
        case LogLevel::LOG_DEBUG:
            prefix = "[ DEBUG ] ";
            break;
        case LogLevel::LOG_WARN:
            prefix = "[ WARN ] ";
            break;
        case LogLevel::LOG_ERROR:
            prefix = "[ ERROR ] ";
            stream = stderr;
            break;
    }

    fprintf(stream, "%s", prefix);
    vfprintf(stream, std::string(msg).c_str(), args);
    fprintf(stream, "\n");
}

void log(LogLevel level, std::string_view msg, ...) {
    va_list args;
    va_start(args, msg);
    v_log(level, msg, args);
    va_end(args);
}

void log_debug(std::string_view msg, ...) {
#ifndef NDEBUG
    va_list args;
    va_start(args, msg);
    v_log(LogLevel::LOG_DEBUG, msg, args);
    va_end(args);
#endif
}

void log_warning(std::string_view msg, ...) {
    va_list args;
    va_start(args, msg);
    v_log(LogLevel::LOG_WARN, msg, args);
    va_end(args);
}

void log_error(std::string_view msg, ...) {
    va_list args;
    va_start(args, msg);
    v_log(LogLevel::LOG_ERROR, msg, args);
    va_end(args);
}
