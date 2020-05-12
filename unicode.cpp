#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include "unicode.h"
#include "utf8.h"
#include "utf16.h"
#include "utf32.h"

std::vector<std::byte> unicode::read_all_bytes_fom_file(const std::string &current_file) {
  std::ifstream raw_file(current_file, std::ios::binary);
  size_t length = std::filesystem::file_size(current_file);
  std::vector<std::byte> data(length);
  raw_file.read(reinterpret_cast<char *>(data.data()), length);
  return data;
}

std::pair<int, std::string> unicode::get_encoding(std::vector<std::byte> &bytes) {
  std::pair<int, std::string> encoding;
  if (bytes[0] == utf8::BOM_FIRST && bytes[1] == utf8::BOM_SECOND && bytes[2] == utf8::BOM_THIRD) {
	encoding = std::make_pair(8, "bom");
	bytes = std::vector(bytes.begin() + 3, bytes.end());
	return encoding;
  } else if (bytes[0] == utf32::BOM_LE_FIRST && bytes[1] == utf32::BOM_LE_SECOND && bytes[2] == unicode::NULL_BYTE &&
	  bytes[3] == unicode::NULL_BYTE) {
	encoding = std::make_pair(32, "le");
	bytes = std::vector(bytes.begin() + 4, bytes.end());
	return encoding;
  } else if (bytes[0] == utf16::BOM_LE_FIRST && bytes[1] == utf16::BOM_LE_SECOND) {
	encoding = std::make_pair(16, "le");
	bytes = std::vector(bytes.begin() + 2, bytes.end());
	return encoding;
  } else if (bytes[0] == unicode::NULL_BYTE && bytes[1] == unicode::NULL_BYTE && bytes[2] == utf32::BOM_BE_THIRD &&
	  bytes[3] == utf32::BOM_BE_FOURTH) {
	encoding = std::make_pair(32, "be");
	bytes = std::vector(bytes.begin() + 4, bytes.end());
	return encoding;
  } else if (bytes[0] == utf16::BOM_BE_FIRST && bytes[1] == utf16::BOM_BE_SECOND) {
	encoding = std::make_pair(16, "be");
	bytes = std::vector(bytes.begin() + 2, bytes.end());
	return encoding;
  }
  return std::make_pair(0, "");
}

size_t unicode::count_code_points(const std::vector<std::vector<std::byte> > &bytes) {
  return bytes.size();
}

size_t unicode::count_code_units(const std::vector<std::byte> &bytes) {
  return bytes.size();
}

size_t unicode::count_words(const std::vector<std::vector<std::byte> > &bytes,
							const std::pair<int, std::string> &encoding) {
  int order = encoding.second == "le" ? -1 : 1;
  std::function<size_t(const std::vector<std::vector<std::byte>> &, int)> utf_8_words_counter{utf8::count_words};
  std::function<size_t(const std::vector<std::vector<std::byte>> &, int)> utf_16_words_counter{utf16::count_words};
  std::function<size_t(const std::vector<std::vector<std::byte>> &, int)> utf_32_words_counter{utf32::count_words};
  std::map<int, std::function<size_t(const std::vector<std::vector<std::byte>> &, int)>> counters
	  = {{8, utf_8_words_counter},
		 {16, utf_16_words_counter},
		 {32, utf_32_words_counter}};
  auto counter = counters[encoding.first];
  auto result = counter(bytes, order);
  return result;
}

std::vector<std::vector<std::byte>>
unicode::normalize_according_to_encoding(const std::vector<std::byte> &bytes, int encoding) {
  std::vector<std::vector<std::byte>> normalized_bytes;
  std::function<std::vector<std::vector<std::byte>>(std::vector<std::byte>)> utf_8_normalizer{utf8::normalize};
  std::function<std::vector<std::vector<std::byte>>(std::vector<std::byte>)> utf_16_normalizer{utf16::normalize};
  std::function<std::vector<std::vector<std::byte>>(std::vector<std::byte>)> utf_32_normalizer{utf32::normalize};
  std::map<int, std::function<std::vector<std::vector<std::byte>>(std::vector<std::byte>)>> normalizers
	  = {{8, utf_8_normalizer},
		 {16, utf_16_normalizer},
		 {32, utf_32_normalizer}};
  auto normalizer = normalizers[encoding];
  normalized_bytes = normalizer(bytes);
  return normalized_bytes;
}

std::vector<std::pair<std::byte, size_t>> unicode::validate(const std::vector<std::byte> &bytes,
															const std::pair<int, std::string> &encoding) {
  std::vector<std::vector<std::byte>> normalized_bytes;
  std::function<std::vector<std::pair<std::byte, size_t>>(const std::vector<std::byte> &, int)>
	  utf8_validator{utf8::validate};
  std::function<std::vector<std::pair<std::byte, size_t>>(const std::vector<std::byte> &, int)>
	  utf16_validator{utf16::validate};
  std::function<std::vector<std::pair<std::byte, size_t>>(const std::vector<std::byte> &, int)>
	  utf32_validator{utf32::validate};
  std::map<int, std::function<std::vector<std::pair<std::byte, size_t>>(const std::vector<std::byte> &, int)>>
	  validators
	  = {{8, utf8_validator},
		 {16, utf16_validator},
		 {32, utf32_validator}};
  auto validator = validators[encoding.first];
  int order = encoding.second == "le" ? -1 : 1;
  auto bad_bits = validator(bytes, order);
  return bad_bits;
}