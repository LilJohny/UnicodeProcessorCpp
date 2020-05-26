//
// Created by denis on 26.04.20.
//
#include <cstddef>
#include <algorithm>
#include "utf16.h"
#include "unicode.h"

inline unsigned short to_short(std::byte byte) {
	return std::to_integer<unsigned short>(byte);
}

inline bool utf16::is_high_surrogate(unsigned short byte_short) {
	return byte_short >= HIGH_SURROGATE_MIN && byte_short <= HIGH_SURROGATE_MAX;
}

inline bool utf16::is_low_surrogate(unsigned short byte_short) {
	return byte_short >= LOW_SURROGATE_MIN && byte_short <= LOW_SURROGATE_MAX;
}
inline int bytes_to_int(std::pair<std::byte, std::byte> bytes) {
	return std::to_integer<int>(bytes.first) << 8u | std::to_integer<int>(bytes.second);
}
inline bool utf16::is_single_unit_point(unsigned short byte_short) {
	return (byte_short >= utf16::SINGLE_UNIT_MIN_1 && byte_short <= utf16::SINGLE_UNIT_MAX_1)
			|| (byte_short >= utf16::SINGLE_UNIT_MIN_2 && byte_short <= utf16::SINGLE_UNIT_MAX_2);
}
bool utf16::is_valid_continuation(std::pair<std::byte, std::byte> previous_pair,
																	std::pair<std::byte, std::byte> current_pair) {
	auto previous_pair_int = bytes_to_int(previous_pair);
	auto current_pair_int = bytes_to_int(current_pair);
	auto single_unit_point = (is_low_surrogate(previous_pair_int) && is_single_unit_point(current_pair_int))
			|| (is_single_unit_point(previous_pair_int) && is_single_unit_point(current_pair_int));
	auto high_surrogate = (is_low_surrogate(previous_pair_int) && is_high_surrogate(current_pair_int))
			|| (is_single_unit_point(previous_pair_int) && is_high_surrogate(current_pair_int));
	auto low_surrogate = is_high_surrogate(previous_pair_int) && is_low_surrogate(current_pair_int);
	return single_unit_point || high_surrogate || low_surrogate;
}
inline bool utf16::is_valid(std::pair<std::byte, std::byte> pair) {
	auto byte_short = bytes_to_int(pair);
	return is_high_surrogate(byte_short) || is_low_surrogate(byte_short) || is_single_unit_point(byte_short);
}
bool utf16::is_space(const std::vector<std::byte> &bytes) {
	for (auto whitespace : WHITESPACES_S) {
		if (bytes[0] == unicode::NULL_BYTE && bytes[1] == whitespace) {
			return true;
		}
	}

	for (const auto &whitespaces : WHITESPACES_D) {
		if (bytes[0] == whitespaces[0] && bytes[1] == whitespaces[1]) {
			return true;
		}
	}

	return false;
}

std::vector<std::vector<std::byte> > utf16::normalize(const std::vector<std::byte> &bytes) {
	std::vector<std::vector<std::byte>> normalized_bytes;
	const int seq_length = 2;
	normalized_bytes.reserve(bytes.size() / seq_length);
	for (int i = 0; i < bytes.size(); i += seq_length) {
		normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + seq_length);
	}
	return normalized_bytes;
}

size_t utf16::count_words(const std::vector<std::vector<std::byte> > &bytes, int order) {
	size_t words_num = 1;
	std::function<std::vector<std::byte>(std::vector<std::byte>)> func;
	if (order == -1) {
		func = [](std::vector<std::byte> bytes) {
			std::reverse(bytes.begin(), bytes.end());
			return bytes;
		};
	} else {
		func = [](const std::vector<std::byte> &bytes) { return bytes; };
	}

	for (int i = 1; i < bytes.size() - 1; ++i) {
		if (utf16::is_space(func(bytes[i]))) {
			i++;
			while (utf16::is_space(func(bytes[i]))) {
				i++;
			}
			words_num++;
		}
	}
	return words_num;
}

std::vector<std::pair<std::byte, size_t>> utf16::validate(std::vector<std::byte> bytes, int order) {
	std::vector<std::pair<std::byte, size_t>> bad_bytes = {};
	size_t cycle_start = 0;
	if (order == -1) {
		for (int i = 0; i < bytes.size(); ++i) {
			auto first = bytes_to_int(std::make_pair(bytes[i], bytes[i + 1]));
			auto second = bytes_to_int(std::make_pair(bytes[i + 2], bytes[i + 3]));
			if (is_low_surrogate(first) && is_high_surrogate(second)) {
				std::swap(bytes[i], bytes[i + 2]);
				std::swap(bytes[i + 1], bytes[i + 3]);
			}
		}
	}
	auto pair = std::make_pair(bytes[cycle_start], bytes[cycle_start + 1]);
	if (!(is_valid(pair) && !is_low_surrogate(bytes_to_int(pair)))) {
		bad_bytes.emplace_back(bytes[cycle_start], cycle_start);
		bad_bytes.emplace_back(bytes[cycle_start + 1], cycle_start + 1);
		cycle_start += 2;
		pair = std::make_pair(bytes[cycle_start], bytes[cycle_start + 1]);
		while (!is_valid(pair)) {
			bad_bytes.emplace_back(bytes[cycle_start], cycle_start);
			bad_bytes.emplace_back(bytes[cycle_start + 1], cycle_start + 1);
			cycle_start += 2;
			pair = std::make_pair(bytes[cycle_start], bytes[cycle_start + 1]);
		}
	}
	for (size_t i = cycle_start + 2; i < bytes.size(); i+=2) {
		auto new_pair = std::make_pair(bytes[i], bytes[i + 1]);
		if (!is_valid_continuation(pair, new_pair)) {
			bad_bytes.emplace_back(bytes[i], i);
			bad_bytes.emplace_back(bytes[i + 1], i + 1);
		}
	}
	return bad_bytes;
}