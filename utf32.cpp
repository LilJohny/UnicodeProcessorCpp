//
// Created by denis on 26.04.20.
//
#include "utf32.h"

bool utf32::is_valid(std::vector<std::byte> bytes) {
    return false;
}

std::vector<std::vector<std::byte>> utf32::normalize(const std::vector<std::byte> &bytes) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    normalized_bytes.reserve(bytes.size() / 4);
    for (int i = 0; i < bytes.size(); i += 4) {
        normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + 4);
    }
    return normalized_bytes;
}
