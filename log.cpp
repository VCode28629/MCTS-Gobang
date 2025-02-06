#include "log.h"

#include <cstdio>

const LogLevel MIN_LOG_LEVEL = Info;

const char *level_to_string(LogLevel level) {
    switch (level) {
        case Debug:
            return "Debug";
        case Info:
            return "Info";
        case Warning:
            return "Warning";
        case Error:
            return "Error";
    }
    return "Unknown";
}

void log(LogLevel level, const std::string &msg) {
    if (level < MIN_LOG_LEVEL) return;
    fprintf(stderr, "[%s]: %s\n", level_to_string(level), msg.c_str());
}