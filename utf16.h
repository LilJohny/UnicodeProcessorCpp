//
// Created by denis on 26.04.20.
//

#ifndef UNICODEPROCESSOR_UTF16_H
#define UNICODEPROCESSOR_UTF16_H

#include <vector>

namespace utf16 {
    const std::byte BOM_BE_FIRST = std::byte('\xfe');
    const std::byte BOM_BE_SECOND = std::byte('\xff');
    const std::byte BOM_LE_FIRST = std::byte('\xff');
    const std::byte BOM_LE_SECOND = std::byte('\xfe');
    const unsigned short HIGH_SURROGATE_MIN = 0xd800;
    const unsigned short HIGH_SURROGATE_MAX = 0xdbff;
    const unsigned short LOW_SURROGATE_MIN = 0xdc00;
    const unsigned short LOW_SURROGATE_MAX = 0xdfff;

    bool is_valid(std::vector<std::byte> bytes);
    bool is_high_surrogate(std::byte byte);
    bool is_low_surrogate(std::byte byte);
    std::vector<std::vector<std::byte>> normalize(const std::vector<std::byte> &bytes);
}
#endif //UNICODEPROCESSOR_UTF16_H
