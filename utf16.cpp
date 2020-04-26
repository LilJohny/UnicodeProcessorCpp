//
// Created by denis on 26.04.20.
//
#include <cstddef>
#include "utf16.h"

bool utf16::is_high_surrogate(std::byte byte) {
    auto byte_short = std::to_integer<unsigned short>(byte);
    return byte_short >= HIGH_SURROGATE_MIN && byte_short <= HIGH_SURROGATE_MAX;
}

bool utf16::is_low_surrogate(std::byte byte) {
    auto byte_short = std::to_integer<unsigned short>(byte);
    return byte_short >= LOW_SURROGATE_MIN && byte_short <= LOW_SURROGATE_MAX;
}

bool utf16::is_valid(std::vector<std::byte> bytes) {
    return false;
}

std::vector<std::vector<std::byte> > utf16::normalize(const std::vector<std::byte> &bytes) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    for (int i = 0; i < bytes.size();) {
        std::byte byte = bytes[0];
        int length = 1;
        if (is_low_surrogate(byte) || is_high_surrogate(byte)) {
            length = 2;
        }
        normalized_bytes.emplace_back(bytes.begin() + i, bytes.end() + i + length);
    }
    return normalized_bytes;
}
