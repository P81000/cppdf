#include "logger.hpp"
#include <cstdarg>
#include <cstdio>

static void v_log(LogLevel level, const char *msg, va_list args) {
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
    vfprintf(stream, msg, args);
    fprintf(stream, "\n");
}

void log(LogLevel level, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    v_log(level, msg, args);
    va_end(args);
}

void log_debug(const char *msg, ...) {
#ifndef NDEBUG
    va_list args;
    va_start(args, msg);
    v_log(LogLevel::LOG_DEBUG, msg, args);
    va_end(args);
#endif
}

void log_warning(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    v_log(LogLevel::LOG_WARN, msg, args);
    va_end(args);
}

void log_error(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    v_log(LogLevel::LOG_ERROR, msg, args);
    va_end(args);
}
