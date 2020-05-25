//
// Created by denis on 26.04.20.
//
#include <stdexcept>
#include "utf8.h"

bool utf8::is_space(const std::vector<std::byte> &bytes) {
	if (bytes.size() == 4) {
		return false;
	} else if (bytes.size() == 3) {
		for (const auto &whitespace : WHITESPACES_T) {
			if (bytes[0] == whitespace[0] && bytes[1] == whitespace[1] && bytes[2] == whitespace[2]) {
				return true;
			}
		}
	} else if (bytes.size() == 2) {
		for (const auto &whitespaces : WHITESPACES_D) {
			if (bytes[0] == whitespaces[0] && bytes[1] == whitespaces[1]) {
				return true;
			}
		}
	} else if (bytes.size() == 1) {
		for (auto whitespaces : WHITESPACES_S) {
			if (bytes[0] == whitespaces) {
				return true;
			}
		}
	}
	return false;
}

std::string to_bin(std::byte current_byte) {
	return std::bitset<8>(std::to_integer<size_t>(current_byte)).to_string();
}

inline bool is_valid_one_byte_unit_first(std::string byte_bin) {
	return byte_bin[0] == '0';
}

inline bool is_valid_two_byte_unit_first(std::string byte_bin) {
	return byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '0';
}

inline bool is_valid_three_byte_unit_first(std::string byte_bin) {
	return byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '1' && byte_bin[3] == '0';
}

inline bool is_valid_four_byte_unit_first(std::string byte_bin) {
	return byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '1' && byte_bin[3] == '1' && byte_bin[4] == '0';
}

inline bool is_valid_continuation_byte(std::string byte_bin) {
	return byte_bin[0] == '1' && byte_bin[1] == '0';
}

std::vector<std::vector<std::byte>> utf8::normalize(const std::vector<std::byte> &bytes) {
	std::vector<std::vector<std::byte>> normalized_bytes;
	int i = 0;
	while (i < bytes.size()) {
		std::byte current_byte = bytes[i];
		auto byte_bin = to_bin(current_byte);
		int length = -1;
		if (is_valid_one_byte_unit_first(byte_bin)) {
			length = 1;
		} else if (is_valid_two_byte_unit_first(byte_bin)) {
			length = 2;
		} else if (is_valid_three_byte_unit_first(byte_bin)) {
			length = 3;
		} else if (is_valid_four_byte_unit_first(byte_bin)) {
			length = 4;
		}
		if (length == -1) {
			throw std::runtime_error("Bad byte");
		}
		normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + length);
		i += length;
	}
	return normalized_bytes;
}

size_t utf8::count_words(const std::vector<std::vector<std::byte>> &bytes, [[maybe_unused]] int order) {
	size_t words_num = 1;
	for (int i = 1; i < bytes.size() - 1; ++i) {
		if (utf8::is_space(bytes[i])) {
			i++;
			while (i < bytes.size() && utf8::is_space(bytes[i])) {
				i++;
			}
			words_num++;
		}
	}
	return words_num;
}

std::vector<int> utf8::validate_batch(const std::vector<std::byte> &batch) {
	std::vector<int> bad_bytes_indexes;
	for (int i = 1; i < batch.size(); ++i) {
		if (!is_valid_continuation_byte(to_bin(batch[i]))) {
			bad_bytes_indexes.push_back(i);
		}
	}
	return bad_bytes_indexes;
}

inline bool utf8::is_valid_start(std::byte byte) {
	auto byte_bin = to_bin(byte);
	return is_valid_one_byte_unit_first(byte_bin) || is_valid_two_byte_unit_first(byte_bin)
			|| is_valid_three_byte_unit_first(byte_bin) || is_valid_four_byte_unit_first(byte_bin);
}

std::vector<std::pair<std::byte, size_t>> utf8::validate(const std::vector<std::byte> &bytes, int order) {
	std::vector<std::pair<std::byte, size_t>> bad_bytes = {};
	size_t i = 0;

	while (!is_valid_start(bytes[i])) {
		bad_bytes.emplace_back(bytes[i], i);
		i++;
	}

	while (i < bytes.size()) {
		int length = -1;
		auto current_byte_bin = to_bin(bytes[i]);
		if (is_valid_one_byte_unit_first(current_byte_bin)) {
			length = 1;
		} else if (is_valid_two_byte_unit_first(current_byte_bin)) {
			length = 2;
		} else if (is_valid_three_byte_unit_first(current_byte_bin)) {
			length = 3;
		} else if (is_valid_four_byte_unit_first(current_byte_bin)) {
			length = 4;
		}
		if (length == -1) {
			bad_bytes.emplace_back(bytes[i], i);
			continue;
		}
		std::vector<std::byte> batch{bytes.begin() + i, bytes.begin() + i + length};
		auto batch_bad_bytes_indices = validate_batch(batch);
		if (!batch_bad_bytes_indices.empty()) {
			for (int j = 0; j < batch_bad_bytes_indices.size(); ++j) {
				bad_bytes.emplace_back(bytes[i + j], i + j);
			}
		}
		i += length;
	}
	return bad_bytes;
}