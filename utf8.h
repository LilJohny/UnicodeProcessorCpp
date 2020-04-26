//
// Created by denis on 26.04.20.
//

#ifndef UNICODEPROCESSOR_UTF8_H
#define UNICODEPROCESSOR_UTF8_H

#include <cstddef>
#include <vector>
namespace utf8 {
    bool is_valid(std::vector<std::byte> bytes);
}
#endif //UNICODEPROCESSOR_UTF8_H
