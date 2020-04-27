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
    const std::vector<std::byte> WHITESPACES_S = {std::byte('\x09'), std::byte('\x0a'), std::byte('\x0b'),
                                                  std::byte('\x0c'), std::byte('\x0d'), std::byte('\x20'),
                                                  std::byte('\x85'), std::byte('\xa0')};
    const std::vector<std::vector<std::byte>> WHITESPACES_D = {
            {std::byte('\x16'), std::byte('\x80')},
            {std::byte('\x20'), std::byte('\x00')},
            {std::byte('\x20'), std::byte('\x01')},
            {std::byte('\x20'), std::byte('\x02')},
            {std::byte('\x20'), std::byte('\x03')},
            {std::byte('\x20'), std::byte('\x04')},
            {std::byte('\x20'), std::byte('\x05')},
            {std::byte('\x20'), std::byte('\x06')},
            {std::byte('\x20'), std::byte('\x07')},
            {std::byte('\x20'), std::byte('\x08')},
            {std::byte('\x20'), std::byte('\x09')},
            {std::byte('\x20'), std::byte('\x0a')},
            {std::byte('\x20'), std::byte('\x28')},
            {std::byte('\x20'), std::byte('\x29')},
            {std::byte('\x20'), std::byte('\x2f')},
            {std::byte('\x20'), std::byte('\x5f')},
            {std::byte('\x30'), std::byte('\x00')},
            {std::byte('\x18'), std::byte('\x0e')},
            {std::byte('\x20'), std::byte('\x0b')},
            {std::byte('\x20'), std::byte('\x0c')},
            {std::byte('\x20'), std::byte('\x0d')},
            {std::byte('\x20'), std::byte('\x60')},
            {std::byte('\xfe'), std::byte('\xff')},
            {std::byte('\x00'), std::byte('\x09')},
            {std::byte('\x00'), std::byte('\x0a')}
    };

    bool is_valid(std::vector<std::byte> bytes);

    bool is_space(const std::vector<std::byte> &bytes);

    size_t count_words(const std::vector<std::vector<std::byte>> &bytes, int order);

    bool is_high_surrogate(std::byte byte);

    bool is_low_surrogate(std::byte byte);

    std::vector<std::vector<std::byte>> normalize(const std::vector<std::byte> &bytes);

    std::vector<std::byte> validate(const std::vector<std::byte> &bytes);

}
#endif //UNICODEPROCESSOR_UTF16_H
