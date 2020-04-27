//
// Created by denis on 26.04.20.
//
#include "utf32.h"
#include "unicode.h"

bool utf32::is_valid(std::vector<std::byte> bytes) {
    return false;
}

bool utf32::is_space(const std::vector<std::byte> &bytes) {
    for (auto whitespace : WHITESPACES_S) {
        if (bytes[0] == unicode::NULL_BYTE && bytes[1] == unicode::NULL_BYTE && bytes[2] == unicode::NULL_BYTE &&
            bytes[3] == whitespace) {
            return true;
        }
    }
    for (const auto &whitespaces : WHITESPACES_D) {
        if (bytes[0] == unicode::NULL_BYTE && bytes[1] == unicode::NULL_BYTE && bytes[2] == whitespaces[0] &&
            bytes[3] == whitespaces[1]) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<std::byte>> utf32::normalize(const std::vector<std::byte> &bytes) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    const int seq_length = 4;
    normalized_bytes.reserve(bytes.size() / seq_length);
    for (int i = 0; i < bytes.size(); i += seq_length) {
        normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + seq_length);
    }
    return normalized_bytes;
}

size_t utf32::count_words(const std::vector<std::vector<std::byte> > &bytes) {
    size_t words_num = 1;
    for (int i = 1; i < bytes.size() - 1; ++i) {
        if (utf32::is_space(bytes[i])) {
            i++;
            while (utf32::is_space(bytes[i])) {
                i++;
            }
            words_num++;
        }
    }
    return words_num;
}