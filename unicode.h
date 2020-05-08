#ifndef UNICODEPROCESSOR_UNICODE_H
#define UNICODEPROCESSOR_UNICODE_H

#include <cstddef>
#include <vector>
#include <string>

namespace unicode {
const std::byte NULL_BYTE = std::byte('\x00');

std::vector<std::vector<std::byte>>
normalize_according_to_encoding(const std::vector<std::byte> &bytes, int encoding);

size_t count_words(const std::vector<std::vector<std::byte>> &bytes, const std::pair<int, std::string> &encoding);

size_t count_code_units(const std::vector<std::byte> &bytes);

size_t count_code_points(const std::vector<std::vector<std::byte>> &bytes);

std::pair<int, std::string> get_encoding(std::vector<std::byte> &bytes);

std::vector<std::byte> read_all_bytes_fom_file(const std::string &current_file);

std::vector<std::pair<std::byte, size_t>> validate(const std::vector<std::byte> &bytes, const std::pair<int, std::string>& encoding);

}
#endif //UNICODEPROCESSOR_UNICODE_H
