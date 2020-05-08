//
// Created by denis on 26.04.20.
//
#include <algorithm>
#include "utf32.h"
#include "unicode.h"

inline bool utf32::is_valid(std::vector<std::byte> bytes) {
  int value_integer =
	  std::to_integer<int>(bytes[0]) << 24u | std::to_integer<int>(bytes[1]) << 16u
		  | std::to_integer<int>(bytes[2]) << 8u
		  | std::to_integer<int>(bytes[3]);

  return value_integer <= MAX_POINT && value_integer >= MIN_POINT;
}

bool utf32::is_space(const std::vector<std::byte> &bytes) {
  for (auto whitespace : WHITESPACES_S) {
	if (bytes[0] == unicode::NULL_BYTE && bytes[1] == unicode::NULL_BYTE && bytes[2] == unicode::NULL_BYTE &&
		bytes[3] == whitespace) {
	  return true;
	}
  }
  for (const auto &whitespaces : WHITESPACES_D) {
	if (bytes[0] == unicode::NULL_BYTE && bytes[1] == unicode::NULL_BYTE && bytes[2] == whitespaces[0] &&
		bytes[3] == whitespaces[1]) {
	  return true;
	}
  }
  return false;
}

std::vector<std::vector<std::byte>> utf32::normalize(const std::vector<std::byte> &bytes) {
  std::vector<std::vector<std::byte>> normalized_bytes;
  const int seq_length = 4;
  normalized_bytes.reserve(bytes.size() / seq_length);
  for (int i = 0; i < bytes.size(); i += seq_length) {
	normalized_bytes.emplace_back(bytes.begin() + i, bytes.begin() + i + seq_length);
  }
  return normalized_bytes;
}

size_t utf32::count_words(const std::vector<std::vector<std::byte> > &bytes, int order) {
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
	if (utf32::is_space(func(bytes[i]))) {
	  i++;
	  while (utf32::is_space(func(bytes[i]))) {
		i++;
	  }
	  words_num++;
	}
  }
  return words_num;
}

std::vector<std::pair<std::byte, size_t>> utf32::validate(const std::vector<std::byte> &bytes, int order) {
  std::vector<std::pair<std::byte, size_t>> bad_bytes = {};
  std::function<std::vector<std::byte>(std::vector<std::byte>)> func;
  if (order == -1) {
	func = [](std::vector<std::byte> bytes) {
	  std::reverse(bytes.begin(), bytes.end());
	  return bytes;
	};
  } else {
	func = [](const std::vector<std::byte> &bytes) { return bytes; };
  }
  for (int i = 0; i < bytes.size(); i += 4) {
	std::vector<std::byte> buffer(bytes.begin() + i, bytes.begin() + i + 4);
	if (!is_valid(func(buffer))) {
	  for (int j = 0; j < 4; ++j) {
		bad_bytes.emplace_back(buffer[j], i + j);
	  }
	}
  }
  return bad_bytes;
}