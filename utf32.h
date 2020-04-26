//
// Created by denis on 26.04.20.
//

#ifndef UNICODEPROCESSOR_UTF32_H
#define UNICODEPROCESSOR_UTF32_H

#include <vector>

namespace utf32 {
    const std::byte BOM_BE_THIRD = std::byte('\xfe');
    const std::byte BOM_BE_FOURTH = std::byte('\xff');
    const std::byte BOM_LE_FIRST = std::byte('\xff');
    const std::byte BOM_LE_SECOND = std::byte('\xfe');
    const std::byte BOM_NULL = std::byte('\x00');
    bool is_valid(std::vector<std::byte> bytes);
    std::vector<std::vector<std::byte>> normalize(const std::vector<std::byte> &bytes);
}
#endif //UNICODEPROCESSOR_UTF32_H
