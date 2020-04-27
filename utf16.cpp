//
// Created by denis on 26.04.20.
//
#include <cstddef>
#include "utf16.h"
#include "unicode.h"

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
    for (auto whitespace : WHITESPACES_S) {
        if (bytes[0] == unicode::NULL_BYTE && bytes[1] == whitespace) {
            return true;
        }
    }

    for (const auto &whitespaces : WHITESPACES_D) {
        if (bytes[0] == whitespaces[0] && bytes[1] == whitespaces[1]) {
            return true;
        }
    }

    return false;
}

std::vector<std::vector<std::byte> > utf16::normalize(const std::vector<std::byte> &bytes) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    const int seq_length = 2;
    normalized_bytes.reserve(bytes.size() / seq_length);
    for (int i = 0; i < bytes.size(); i += seq_length) {
        normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + seq_length);
    }
    return normalized_bytes;
}

size_t utf16::count_words(const std::vector<std::vector<std::byte> > &bytes) {
    size_t words_num = 1;
    for (int i = 1; i < bytes.size() - 1; ++i) {
        if (utf16::is_space(bytes[i])) {
            i++;
            while (utf16::is_space(bytes[i])) {
                i++;
            }
            words_num++;
        }
    }
    return words_num;
}

std::vector<std::byte> utf16::validate(const std::vector<std::byte> &bytes) {
    return {};
}