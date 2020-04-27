#include <iostream>
#include "unicode.h"

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


std::pair<int, std::string> parse_encoding(const std::string& encoding_str) {
    return std::make_pair(1,"");
}

int main(int argc, char *argv[]) {
    auto args = parse_args(argc, argv);
    auto input_file = args.first;
    auto encoding = args.second;
    auto file_bytes = unicode::read_all_bytes_fom_file(input_file);
    auto deducted_encoding = unicode::get_encoding(file_bytes);
    std::pair<int, std::string> file_encoding;
    if (deducted_encoding.first == 0 && encoding.empty()) {
        throw std::runtime_error("File is encoded without BOM , so program needs encoding as its second argument, but it was`nt supplied!");
    } else if(deducted_encoding.first==0 && !encoding.empty()){
        file_encoding = parse_encoding(encoding);
    } else {
        file_encoding = deducted_encoding;
    }

    auto bad_bits = unicode::validate(file_bytes, file_encoding.first);
    if (!bad_bits.empty()) {
        std::cout << input_file << "has bad encoding." << std::endl;
        std::cout << "Number of bad bits: " << bad_bits.size() << std::endl;
        std::cout << "List of bad bits :" << std::endl;
        for (auto &bad_bit : bad_bits) {
            std::cout << std::hex << std::to_integer<int>(bad_bit) << std::endl;
        }
        return 1;
    }
    std::cout << "Code units number: " << unicode::count_code_units(file_bytes) << std::endl;
    auto normalized_bytes = unicode::normalize_according_to_encoding(file_bytes, file_encoding.first);
    std::cout << "Code points number: " << unicode::count_code_points(normalized_bytes) << std::endl;
    std::cout << "Words number: " << unicode::count_words(normalized_bytes, file_encoding) << std::endl;
    return 0;
}
