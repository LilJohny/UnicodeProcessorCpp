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
inline bool utf16::is_single_unit_point(unsigned short byte_short) {
  return (byte_short >= utf16::SINGLE_UNIT_MIN_1 && byte_short <= utf16::SINGLE_UNIT_MAX_1)
	  || (byte_short >= utf16::SINGLE_UNIT_MIN_2 && byte_short <= utf16::SINGLE_UNIT_MAX_2);
}
bool utf16::is_valid(std::byte previous_byte, std::byte current_byte) {
  auto previous_byte_short = to_short(previous_byte);
  auto current_byte_short = to_short(current_byte);
  auto single_unit_point = (is_low_surrogate(previous_byte_short) && is_single_unit_point(current_byte_short))
	  || (is_single_unit_point(previous_byte_short) && is_single_unit_point(current_byte_short));
  auto high_surrogate = (is_low_surrogate(previous_byte_short) && is_high_surrogate(current_byte_short))
	  || (is_single_unit_point(previous_byte_short) && is_high_surrogate(current_byte_short));
  auto low_surrogate = is_high_surrogate(previous_byte_short) && is_low_surrogate(current_byte_short);
  return single_unit_point || high_surrogate || low_surrogate;
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

std::vector<std::pair<std::byte, size_t>> utf16::validate(const std::vector<std::byte> &bytes) {
  return {};
}