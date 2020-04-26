//
// Created by denis on 26.04.20.
//

#ifndef UNICODEPROCESSOR_UTF8_H
#define UNICODEPROCESSOR_UTF8_H

#include <cstddef>
#include <vector>
#include <bitset>
#include "utf8.h"
namespace utf8 {
    const std::byte BOM_FIRST = std::byte('\xef');
    const std::byte BOM_SECOND = std::byte('\xbb');
    const std::byte BOM_THIRD = std::byte('\xbf');
    bool is_valid(std::vector<std::byte> bytes);
    std::vector<std::vector<std::byte>> normalize(const std::vector<std::byte>& bytes);
}
#endif //UNICODEPROCESSOR_UTF8_H
