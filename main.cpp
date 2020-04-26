#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <filesystem>
#include <map>
#include <functional>
#include "utf8.h"
#include "utf16.h"
#include "utf32.h"


std::pair<std::string, std::string> parse_args(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "You need to specify file  as first argument";
        throw std::runtime_error("File is not specified");
    } else if (argc == 2) {
        return std::make_pair(argv[1], "");
    } else if (argc == 3) {
        return std::make_pair(argv[1], argv[2]);
    }
    return std::make_pair("", "");
}

std::vector<std::byte> read_all_bytes_fom_file(const std::string &current_file) {
    std::ifstream raw_file(current_file, std::ios::binary);
    size_t length = std::filesystem::file_size(current_file);
    std::vector<std::byte> data(length);
    raw_file.read(reinterpret_cast<char *>(data.data()), length);
    return data;
}

std::pair<int, std::string> get_encoding(std::vector<std::byte> &bytes) {
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
    } else if (bytes[0] == utf32::BOM_LE_FIRST && bytes[1] == utf32::BOM_LE_SECOND && bytes[2] == utf32::BOM_NULL &&
               bytes[3] == utf32::BOM_NULL) {
        encoding = std::make_pair(32, "le");
        bytes = std::vector(bytes.begin() + 4, bytes.end());
        return encoding;
    } else if (bytes[0] == utf32::BOM_NULL && bytes[1] == utf32::BOM_NULL && bytes[2] == utf32::BOM_BE_THIRD &&
               bytes[3] == utf32::BOM_BE_FOURTH) {
        encoding = std::make_pair(32, "be");
        bytes = std::vector(bytes.begin() + 4, bytes.end());
        return encoding;
    }
    return std::make_pair(0, "");
}

size_t count_code_points(std::vector<std::vector<std::byte>> bytes, std::pair<std::string, std::string> encoding) {
    return bytes.size();
}

size_t count_code_units(std::vector<std::vector<std::byte>> bytes, std::pair<std::string, std::string> encoding) {
    return 0;
}

size_t count_words(std::vector<std::vector<std::byte>> bytes, std::pair<std::string, std::string> encoding) {
    return 0;
}


auto normalize_according_to_encoding(const std::vector<std::byte> &bytes, int encoding) {
    std::vector<std::vector<std::byte>> normalized_bytes;
    std::map<int, std::function<std::vector<std::vector<std::byte>>(std::vector<std::byte>)>> normalizers
            = {{8,  std::function(utf8::normalize)},
               {16, std::function(utf16::normalize)},
               {32, std::function(utf32::normalize)}};
    auto normalizer = normalizers[encoding];
    normalized_bytes = normalizer(bytes);
    return normalized_bytes;
}

int main(int argc, char *argv[]) {
    auto args = parse_args(argc, argv);
    auto input_file = args.first;
    auto encoding = args.second;
    auto file_bytes = read_all_bytes_fom_file(input_file);
    auto deducted_encoding = get_encoding(file_bytes);
    if (deducted_encoding.first == 0) {
        throw std::runtime_error("Dont supported file encoding");
    }
    return 0;
}
