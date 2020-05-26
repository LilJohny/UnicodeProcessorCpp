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
std::vector<std::vector<std::byte>> get_first_byte_boundaries(std::vector<std::vector<std::vector<std::byte>>> current_boundaries) {
	std::vector<std::vector<std::byte>> first_byte_boundaries = {};
	first_byte_boundaries.reserve(current_boundaries.size());
	for (auto &current_boundary : current_boundaries) {
		first_byte_boundaries.emplace_back(current_boundary[0]);
	}
	return first_byte_boundaries;
}
bool satisfies_boundary(std::byte byte, std::vector<std::byte> boundary) {
	return (boundary.size() == 1 && byte == boundary[0])
			|| (boundary.size() == 2 && byte >= boundary[0] && byte <= boundary[1]);
}
bool check_first_byte_boundary_condition(std::byte byte, const std::vector<std::vector<std::byte>> &boundaries) {
	bool result = false;
	for (auto &boundary : boundaries) {
		if (satisfies_boundary(byte, boundary)) {
			result = true;
			break;
		}
	}
	return result;
}
inline bool is_valid_one_byte_unit_first(std::byte byte) {
	auto byte_bin = to_bin(byte);
	auto bin_condition = byte_bin[0] == '0';
	auto current_boundaries = utf8::boundaries.at(1);
	std::vector<std::vector<std::byte>> first_byte_boundaries = get_first_byte_boundaries(current_boundaries);
	auto first_byte_boundary_condition = check_first_byte_boundary_condition(byte, first_byte_boundaries);
	return bin_condition && first_byte_boundary_condition;
}

inline bool is_valid_two_byte_unit_first(std::byte byte) {
	auto byte_bin = to_bin(byte);
	auto bin_condition = byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '0';
	auto current_boundaries = utf8::boundaries.at(2);
	std::vector<std::vector<std::byte>> first_byte_boundaries = get_first_byte_boundaries(current_boundaries);
	auto first_byte_boundary_condition = check_first_byte_boundary_condition(byte, first_byte_boundaries);
	return bin_condition && first_byte_boundary_condition;
}

inline bool is_valid_three_byte_unit_first(std::byte byte) {
	auto byte_bin = to_bin(byte);
	auto bin_condition = byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '1' && byte_bin[3] == '0';
	auto current_boundaries = utf8::boundaries.at(3);
	std::vector<std::vector<std::byte>> first_byte_boundaries = get_first_byte_boundaries(current_boundaries);
	auto first_byte_boundary_condition = check_first_byte_boundary_condition(byte, first_byte_boundaries);
	return bin_condition && first_byte_boundary_condition;
}

inline bool is_valid_four_byte_unit_first(std::byte byte) {
	auto byte_bin = to_bin(byte);
	auto bin_condition =
			byte_bin[0] == '1' && byte_bin[1] == '1' && byte_bin[2] == '1' && byte_bin[3] == '1' && byte_bin[4] == '0';
	auto current_boundaries = utf8::boundaries.at(4);
	std::vector<std::vector<std::byte>> first_byte_boundaries = get_first_byte_boundaries(current_boundaries);
	auto first_byte_boundary_condition = check_first_byte_boundary_condition(byte, first_byte_boundaries);
	return bin_condition && first_byte_boundary_condition;
}

std::vector<std::vector<std::byte>> utf8::normalize(const std::vector<std::byte> &bytes) {
	std::vector<std::vector<std::byte>> normalized_bytes;
	int i = 0;
	while (i < bytes.size()) {
		std::byte current_byte = bytes[i];
		int length = -1;
		if (is_valid_one_byte_unit_first(current_byte)) {
			length = 1;
		} else if (is_valid_two_byte_unit_first(current_byte)) {
			length = 2;
		} else if (is_valid_three_byte_unit_first(current_byte)) {
			length = 3;
		} else if (is_valid_four_byte_unit_first(current_byte)) {
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
	auto batch_boundaries = utf8::boundaries.at(batch.size());
	for (int j = 0; j < batch.size(); ++j) {
		bool not_valid = true;
		for (auto &batch_boundary : batch_boundaries) {
			if (satisfies_boundary(batch[j], batch_boundary[j])) {
				not_valid = false;
				break;
			}
		}
		if (not_valid) {
			bad_bytes_indexes.emplace_back(j);
		}
	}
	return bad_bytes_indexes;
}

inline bool utf8::is_valid_start(std::byte byte) {
	return is_valid_one_byte_unit_first(byte) || is_valid_two_byte_unit_first(byte)
			|| is_valid_three_byte_unit_first(byte) || is_valid_four_byte_unit_first(byte);
}
void bad_bytes_indices_post_processing(std::vector<int> &bad_bytes_indices, const std::vector<std::byte> &batch) {
	if (batch.size() - bad_bytes_indices.size() == 1
			&& std::find(bad_bytes_indices.begin(), bad_bytes_indices.end(), 0) == bad_bytes_indices.end()) {
		bad_bytes_indices = {0};
	}
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
		if (is_valid_one_byte_unit_first(bytes[i])) {
			length = 1;
		} else if (is_valid_two_byte_unit_first(bytes[i])) {
			length = 2;
		} else if (is_valid_three_byte_unit_first(bytes[i])) {
			length = 3;
		} else if (is_valid_four_byte_unit_first(bytes[i])) {
			length = 4;
		}
		if (length == -1) {
			bad_bytes.emplace_back(bytes[i], i);
			continue;
		}
		std::vector<std::byte> batch{bytes.begin() + i, bytes.begin() + i + length};
		auto batch_bad_bytes_indices = validate_batch(batch);
		if (!batch_bad_bytes_indices.empty()) {
			bad_bytes_indices_post_processing(batch_bad_bytes_indices, batch);
			for (auto bad_bytes_index : batch_bad_bytes_indices) {
				bad_bytes.emplace_back(bytes[i + bad_bytes_index], i + bad_bytes_index);
			}
		}
		i += length;
	}
	return bad_bytes;
}