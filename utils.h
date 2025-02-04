#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>
#include <iostream>
#include <chrono>
#include <random>

std::string fstring(const char* format, ...);

std::string durationToString(const std::chrono::duration<double>& duration);

int random(int length);

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 16);
    }
};