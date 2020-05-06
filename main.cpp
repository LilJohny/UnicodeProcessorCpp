#include <iostream>
#include "unicode.h"
void show_help() {
  std::cout << "Help text here" << std::endl;
};
std::pair<std::string, std::string> parse_args(int argc, std::vector<std::string> argv) {
  if (argc == 2 && argv[1] == "-h") {
	show_help();
  }
  else if (argc < 3) {
	std::cerr << "Incorrect format of program call";
	std::cout << "Example of program use:" << std::endl;
	std::cout << "./unicode -file text_filename [-encoding encoding_name]" << std::endl << "or" << std::endl;
	std::cout << "./unicode -h" << "To show detailed help of program usage" << std::endl;
	throw std::runtime_error("Incorrect format of program call");
  }
  return std::make_pair("", "");
}

std::pair<int, std::string> parse_encoding(const std::string &encoding_str) {
  return std::make_pair(1, "");
}

int main(int argc, char *argv[]) {
  std::vector<std::string> argv_vector;
  argv_vector.reserve(argc);
  for (int i = 0; i < argc; ++i) {
	argv_vector.emplace_back(argv[i]);
  }
  auto args = parse_args(argc, argv_vector);
  auto input_file = args.first;
  auto encoding = args.second;
  auto file_bytes = unicode::read_all_bytes_fom_file(input_file);
  auto deducted_encoding = unicode::get_encoding(file_bytes);
  std::pair<int, std::string> file_encoding;
  if (deducted_encoding.first == 0 && encoding.empty()) {
	throw std::runtime_error(
		"File is encoded without BOM , so program needs encoding as its second argument, but it was`nt supplied!");
  } else if (deducted_encoding.first == 0 && !encoding.empty()) {
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
