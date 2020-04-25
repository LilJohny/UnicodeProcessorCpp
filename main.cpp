#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>
#include <filesystem>

auto read_all_bytes_fom_file(const std::string &current_file) {
    std::ifstream raw_file(current_file, std::ios::binary);
    size_t length = std::filesystem::file_size(current_file);
    std::vector<std::byte> data(length);
    raw_file.read(reinterpret_cast<char *>(data.data()), length);
    return data;
}


int main() {
    auto file = read_all_bytes_fom_file("../total_value.txt");
    std::cout<<(static_cast<int>(file[0]) == '\x70')<<"\n";

    return 0;
}
