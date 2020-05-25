#include <iostream>
#include <filesystem>
#include "unicode.h"
#include <algorithm>
void show_help() {
	std::cout << "Help text here" << std::endl;
}

void incorrect_input_handler() {
	std::cerr << "Incorrect format of program call";
	std::cout << "Example of program use:" << std::endl;
	std::cout << "./unicode -file text_filename [-encoding encoding_name]" << std::endl << "or" << std::endl;
	std::cout << "./unicode -h" << "To show detailed help of program usage" << std::endl;
	throw std::runtime_error("Incorrect program call");
}

std::pair<std::string, std::string> parse_args(int argc, std::vector<std::string> argv) {
	if (argc == 2 && argv[1] == "-h") {
		show_help();
		exit(0);
	} else if (argc < 3 || argc > 5) {
		incorrect_input_handler();
	} else {
		std::string filename;
		std::string encoding;
		for (int i = 1; i < argc; ++i) {
			if (argv[i] == "-file") {
				filename = argv[i + 1];
				i++;
			} else if (argv[i] == "-encoding") {
				encoding = argv[i + 1];
				i++;
			} else if (argv[i - 1][0] != '-' && argv[i][0] != '-') {
				filename = argv[i];
			} else if (argv[i][0] == '-') {
				throw std::runtime_error("Option " + argv[i] + " is invalid");
			}
		}
		if (filename.empty()) {
			incorrect_input_handler();
		}
		if (!std::filesystem::exists(filename)) {
			throw std::runtime_error("File " + filename + " does not exist");
		}
		return std::make_pair(filename, encoding);
	}
	return std::make_pair("", "");
}

std::pair<int, std::string> parse_encoding(const std::string &encoding_str) {
	std::pair<int, std::string> parsed_encoding = std::make_pair(1, "");
	if (encoding_str == "utf8") {
		parsed_encoding = std::make_pair(8, "");
	} else if (encoding_str == "utf8bom") {
		parsed_encoding = std::make_pair(8, "bom");
	} else if (encoding_str == "utf16be") {
		parsed_encoding = std::make_pair(16, "be");
	} else if (encoding_str == "utf16le") {
		parsed_encoding = std::make_pair(16, "le");
	} else if (encoding_str == "utf32be") {
		parsed_encoding = std::make_pair(32, "be");
	} else if (encoding_str == "utf32le") {
		parsed_encoding = std::make_pair(32, "le");
	}
	if (parsed_encoding.first == 1) {
		throw std::runtime_error(encoding_str + " is not valid encoding name.");
	}
	return parsed_encoding;
}

int main(int argc, char *argv[]) {
	std::vector<std::string> argv_vector;
	argv_vector.reserve(argc);
	for (int i = 0; i < argc; ++i) {
		argv_vector.emplace_back(argv[i]);
	}
	std::pair<std::string, std::string> args;
	try {
		args = parse_args(argc, argv_vector);
	} catch (std::runtime_error &ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	auto input_file = args.first;
	auto encoding = args.second;
	auto file_bytes = unicode::read_all_bytes_fom_file(input_file);
	auto deducted_encoding = unicode::get_encoding(file_bytes);
	std::pair<int, std::string> file_encoding;
	if (deducted_encoding.first == 0 && encoding.empty()) {
		throw std::runtime_error(
				"File is encoded without BOM , so program needs encoding as its second argument, but it was`nt supplied!");
	} else if (deducted_encoding.first == 0 && !encoding.empty()) {

		try {
			file_encoding = parse_encoding(encoding);
		} catch (std::runtime_error &ex) {
			std::cout << ex.what() << std::endl;
			std::vector<std::string> valid_encodings = {"utf8", "utf8bom", "utf16be", "utf16le", "utf32be", "utf32le"};
			std::cout << "List of valid encodings: " << std::endl;
			for (int i = 0; i < valid_encodings.size() - 1; ++i) {
				std::cout << valid_encodings[i] << std::endl;
			}
			std::cout << valid_encodings[valid_encodings.size() - 1];
			return 2;
		}

	} else {
		file_encoding = deducted_encoding;
	}

	auto bad_bytes = unicode::validate(file_bytes, file_encoding);
	if (!bad_bytes.empty()) {
		std::cout << input_file << "has bad encoding." << std::endl;
		std::cout << "Number of bad bits: " << bad_bytes.size() << std::endl;
		std::cout << "List of bad bits :" << std::endl;
		for (auto &bad_byte : bad_bytes) {
			std::cout << std::hex << std::to_integer<int>(bad_byte.first);
			std::cout << std::dec << " at " << bad_byte.second << " position" << std::endl;
		}
		return 3;
	}
	std::cout << "Code units number: " << unicode::count_code_units(file_bytes) << std::endl;
	auto normalized_bytes = unicode::normalize_according_to_encoding(file_bytes, file_encoding.first);
	std::cout << "Code points number: " << unicode::count_code_points(normalized_bytes) << std::endl;
	std::cout << "Words number: " << unicode::count_words(normalized_bytes, file_encoding) << std::endl;
	return 0;
}
