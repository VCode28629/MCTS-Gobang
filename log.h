#pragma once

#include<string>

enum LogLevel {
    Debug,
    Info,
    Warning,
    Error,
};

void log(LogLevel, const std::string &msg);