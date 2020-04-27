//
// Created by denis on 26.04.20.
//
#include "utf8.h"

bool utf8::is_valid(std::vector<std::byte> bytes) {
    return false;
}

bool utf8::is_space(const std::vector<std::byte> &bytes) {
    if (bytes.size() == 4) {
        return false;
    } else if (bytes.size() == 3) {
        for (const auto &whitespace : WHITESPACES_T) {
            if (bytes[0] == whitespace[0] && bytes[1] == whitespace[1] && bytes[2] == whitespace[2]) {
                return true;
            }
        }
    } else if (bytes.size() == 2) {
        for (const auto &whitespaces : WHITESPACES_D) {
            if (bytes[0] == whitespaces[0] && bytes[1] == whitespaces[1]) {
                return true;
            }
        }
    } else if (bytes.size() == 1) {
        for (auto whitespaces : WHITESPACES_S) {
            if (bytes[0] == whitespaces) {
                return true;
            }
        }
    }
    return false;
}


std::vector<std::vector<std::byte>> utf8::normalize(const std::vector<std::byte> &bytes) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    int i = 0;
    while (i < bytes.size()) {
        std::byte current_byte = bytes[i];
        auto byte_bin = std::bitset<8>(std::to_integer<size_t>(current_byte)).to_string();
        int length;
        if (byte_bin[0] == '0') {
            length = 1;
        } else if (byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '0') {
            length = 2;
        } else if (byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '1' && byte_bin[2] == '0') {
            length = 3;
        } else if (byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '1' && byte_bin[2] == '1' &&
                   byte_bin[2] == '0') {
            length = 4;
        }
        normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + length);
        i += length;
    }
    return normalized_bytes;
}

size_t utf8::count_words(const std::vector<std::vector<std::byte>> &bytes) {
    size_t words_num = 1;
    for (int i = 1; i < bytes.size() - 1; ++i) {
        if (utf8::is_space(bytes[i])) {
            i++;
            while (utf8::is_space(bytes[i])) {
                i++;
            }
            words_num++;
        }
    }
    return words_num;
}

std::vector<std::byte> utf8::validate(const std::vector<std::byte> &bytes) {
    return {};
}