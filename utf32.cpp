//
// Created by denis on 26.04.20.
//
#include "utf32.h"

bool utf32::is_valid(std::vector<std::byte> bytes) {
    return false;
}

bool utf32::is_space(const std::vector<std::byte> &bytes) {
    for (auto whitespace : WHITESPACES_S) {
        if (bytes[0] == NULL_BYTE && bytes[1] == NULL_BYTE && bytes[2] == NULL_BYTE && bytes[3] == whitespace) {
            return true;
        }
    }
    for (const auto &whitespaces : WHITESPACES_D) {
        if (bytes[0] == NULL_BYTE && bytes[1] == NULL_BYTE && bytes[2] == whitespaces[0] &&
            bytes[3] == whitespaces[1]) {
            return true;
        }
    }
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
