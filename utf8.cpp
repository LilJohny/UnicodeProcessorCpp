//
// Created by denis on 26.04.20.
//
#include "utf8.h"

bool utf8::is_valid(std::vector<std::byte> bytes) {
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
        normalized_bytes.emplace_back(bytes.begin(), bytes.begin()+length);
    }
    return normalized_bytes;
}