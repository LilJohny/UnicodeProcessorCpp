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

bool utf16::is_space(const std::vector<std::byte> &bytes) {
    if (bytes.size() == 1) {
        for (auto whitespace : WHITESPACES_S) {
            if (bytes[0] == whitespace) {
                return true;
            }
        }
    } else if (bytes.size() == 2) {
        for (const auto &whitespaces : WHITESPACES_D) {
            if (bytes[0] == whitespaces[0] && bytes[1] == whitespaces[1]) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::vector<std::byte> > utf16::normalize(const std::vector<std::byte> &bytes) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    for (int i = 0; i < bytes.size();) {
        std::byte byte = bytes[0];
        int length = 1;
        if (utf16::is_low_surrogate(byte) || utf16::is_high_surrogate(byte)) {
            normalized_bytes.emplace_back(bytes.begin() + i, bytes.end() + i + 2);
            length = 2;
        }
        normalized_bytes.emplace_back(1, byte);
        i += length;
    }
    return normalized_bytes;
}
