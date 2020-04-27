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
    for (int i = 0; i < bytes.size();) {
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
        normalized_bytes.emplace_back(bytes.begin(), bytes.begin() + length);
        i += length;
    }
    return normalized_bytes;
}