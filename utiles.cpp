#include "utils.h"

std::string fstring(const char *format, ...) {
    va_list args;
    va_start(args, format);
    const int length = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (length < 0) {
        return {};
    }

    va_start(args, format);
    std::string result(length, '\0');
    vsnprintf(result.data(), length + 1, format, args);
    va_end(args);

    return result;
}

std::string durationToString(const std::chrono::duration<double> &duration) {
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return fstring("%d.%ds", milliseconds / 1000, milliseconds % 1000);
}

int random(int length) {  // [0, length)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, length - 1);
    return dis(gen);
}