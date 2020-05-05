#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include "unicode.h"
#include "utf8.h"
#include "utf16.h"
#include "utf32.h"

std::vector<std::byte> unicode::read_all_bytes_fom_file(const std::string &current_file) {
    std::ifstream raw_file(current_file, std::ios::binary);
    size_t length = std::filesystem::file_size(current_file);
    std::vector<std::byte> data(length);
    raw_file.read(reinterpret_cast<char *>(data.data()), length);
    return data;
}

std::pair<int, std::string> unicode::get_encoding(std::vector<std::byte> &bytes) {
    std::pair<int, std::string> encoding;
    if (bytes[0] == utf8::BOM_FIRST && bytes[1] == utf8::BOM_SECOND && bytes[2] == utf8::BOM_THIRD) {
        encoding = std::make_pair(8, "");
        bytes = std::vector(bytes.begin() + 3, bytes.end());
        return encoding;
    } else if (bytes[0] == utf16::BOM_LE_FIRST && bytes[1] == utf16::BOM_LE_SECOND) {
        encoding = std::make_pair(16, "le");
        bytes = std::vector(bytes.begin() + 2, bytes.end());
        return encoding;
    } else if (bytes[0] == utf16::BOM_BE_FIRST && bytes[1] == utf16::BOM_BE_SECOND) {
        encoding = std::make_pair(16, "be");
        bytes = std::vector(bytes.begin() + 2, bytes.end());
        return encoding;
    } else if (bytes[0] == utf32::BOM_LE_FIRST && bytes[1] == utf32::BOM_LE_SECOND && bytes[2] == unicode::NULL_BYTE &&
               bytes[3] == unicode::NULL_BYTE) {
        encoding = std::make_pair(32, "le");
        bytes = std::vector(bytes.begin() + 4, bytes.end());
        return encoding;
    } else if (bytes[0] == unicode::NULL_BYTE && bytes[1] == unicode::NULL_BYTE && bytes[2] == utf32::BOM_BE_THIRD &&
               bytes[3] == utf32::BOM_BE_FOURTH) {
        encoding = std::make_pair(32, "be");
        bytes = std::vector(bytes.begin() + 4, bytes.end());
        return encoding;
    }
    return std::make_pair(0, "");
}

size_t unicode::count_code_points(const std::vector<std::vector<std::byte> > &bytes) {
    return bytes.size();
}

size_t unicode::count_code_units(const std::vector<std::byte> &bytes) {
    return bytes.size();
}

size_t unicode::count_words(const std::vector<std::vector<std::byte> > &bytes, const std::pair<int, std::string>& encoding) {
    int order = encoding.second == "be" ? -1 : 1;
    std::map<int, std::function<size_t(const std::vector<std::vector<std::byte>> &, int)>> counters
            = {{8,  std::function(utf8::count_words)},
               {16, std::function(utf16::count_words)},
               {32, std::function(utf32::count_words)}};
    auto counter = counters[encoding.first];
    auto result = counter(bytes, order);
    return result;
}

std::vector<std::vector<std::byte>>
unicode::normalize_according_to_encoding(const std::vector<std::byte> &bytes, int encoding) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    std::map<int, std::function<std::vector<std::vector<std::byte>>(std::vector<std::byte>)>> normalizers
            = {{8,  std::function(utf8::normalize)},
               {16, std::function(utf16::normalize)},
               {32, std::function(utf32::normalize)}};
    auto normalizer = normalizers[encoding];
    normalized_bytes = normalizer(bytes);
    return normalized_bytes;
}

std::vector<std::byte> unicode::validate(const std::vector<std::byte> &bytes, int encoding) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    std::map<int, std::function<std::vector<std::byte>(const std::vector<std::byte> &)>> validators
            = {{8,  std::function(utf8::validate)},
               {16, std::function(utf16::validate)},
               {32, std::function(utf32::validate)}};
    auto validator = validators[encoding];
    auto bad_bits = validator(bytes);
    return bad_bits;
}