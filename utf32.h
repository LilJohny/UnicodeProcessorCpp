//
// Created by denis on 26.04.20.
//

#ifndef UNICODEPROCESSOR_UTF32_H
#define UNICODEPROCESSOR_UTF32_H

#include <vector>

namespace utf32 {
    bool is_valid(std::vector<std::byte> bytes);
}
#endif //UNICODEPROCESSOR_UTF32_H
