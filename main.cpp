#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>
#include <filesystem>
#include <map>
#include <functional>

using byte = std::byte;

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

auto read_all_bytes_fom_file(const std::string &current_file) {
    std::ifstream raw_file(current_file, std::ios::binary);
    size_t length = std::filesystem::file_size(current_file);
    std::vector<std::byte> data(length);
    raw_file.read(reinterpret_cast<char *>(data.data()), length);
    return data;
}

std::pair<int, std::string> get_encoding(std::vector<std::byte> &bytes) {
    std::pair<int, std::string> encoding;
    if (bytes[0] == byte('\xef') && bytes[1] == byte('\xbb') && bytes[2] == byte('\xbf')) {
        encoding = std::make_pair(8, "");
        bytes = std::vector(bytes.begin() + 3, bytes.end());
        return encoding;
    } else if (bytes[0] == byte('\xff') && bytes[1] == byte('\xfe')) {
        encoding = std::make_pair(16, "le");
        bytes = std::vector(bytes.begin() + 2, bytes.end());
        return encoding;
    } else if (bytes[0] == byte('\xfe') && bytes[1] == byte('\xff')) {
        encoding = std::make_pair(16, "be");
        bytes = std::vector(bytes.begin() + 2, bytes.end());
        return encoding;
    } else if (bytes[0] == byte('\xff') && bytes[1] == byte('\xfe') && bytes[2] == byte('\x00') &&
               bytes[3] == byte('\x00')) {
        encoding = std::make_pair(16, "le");
        bytes = std::vector(bytes.begin() + 4, bytes.end());
        return encoding;
    } else if (bytes[0] == byte('\x00') && bytes[1] == byte('\x00') && bytes[2] == byte('\xfe') &&
               bytes[3] == byte('\xff')) {
        encoding = std::make_pair(16, "be");
        bytes = std::vector(bytes.begin() + 4, bytes.end());
        return encoding;
    }
    return std::make_pair(0, "");
}

size_t count_code_points(std::vector<std::vector<byte>> bytes, std::pair<std::string, std::string> encoding) {
    return bytes.size();
}

size_t count_code_units(std::vector<std::vector<byte>> bytes, std::pair<std::string, std::string> encoding) {
    return 0;
}

size_t count_words(std::vector<std::vector<byte>> bytes, std::pair<std::string, std::string> encoding) {
    return 0;
}

auto normalize_utf8(const std::vector<byte> &bytes) {
    std::vector<std::vector<byte>> normalized_bytes;
    return normalized_bytes;
}

auto normalize_utf16(const std::vector<byte> &bytes) {
    std::vector<std::vector<byte>> normalized_bytes;
//    for (int i = 0; i < bytes.size();) {
//        if()
//    }
    return normalized_bytes;
}

auto normalize_utf32(const std::vector<byte> &bytes) {
    std::vector<std::vector<byte>> normalized_bytes;
    normalized_bytes.reserve(bytes.size()/4);
    for (int i = 0; i < bytes.size(); i += 4) {
        normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + 4);
    }
    return normalized_bytes;
}

auto normalize_according_to_encoding(const std::vector<byte> &bytes, int encoding) {
    std::vector<std::vector<byte>> normalized_bytes;
    std::map<int, std::function<std::vector<std::vector<byte>>(std::vector<byte>)>> normalizers
            = {{8,  std::function(normalize_utf8)},
               {16, std::function(normalize_utf16)},
               {32, std::function(normalize_utf32)}};
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
