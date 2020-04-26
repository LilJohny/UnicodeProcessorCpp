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

    bool is_valid(std::vector<std::byte> bytes);
    std::vector<std::vector<std::byte>> normalize(std::vector<std::byte> bytes);
}
#endif //UNICODEPROCESSOR_UTF16_H
