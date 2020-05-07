//
// Created by denis on 26.04.20.
//

#ifndef UNICODEPROCESSOR_UTF8_H
#define UNICODEPROCESSOR_UTF8_H

#include <cstddef>
#include <vector>
#include <bitset>
#include "utf8.h"

namespace utf8 {
const std::byte BOM_FIRST = std::byte('\xef');
const std::byte BOM_SECOND = std::byte('\xbb');
const std::byte BOM_THIRD = std::byte('\xbf');

const int QUADRO_UNIT_MAX = 1114111;
const int QUADRO_UNIT_MIN = 65536;
const int TRIPLE_UNIT_MAX = 65535;
const int TRIPLE_UNIT_MIN = 2048;
const int DOUBLE_UNIT_MAX = 2047;
const int DOUBLE_UNIT_MIN = 128;
const int SINGLE_UNIT_MAX = 127;
const int SINGLE_UNIT_MIN = 0;

const std::vector<std::byte> WHITESPACES_S = {std::byte('\x09'), std::byte('\x0a'), std::byte('\x0b'),
											  std::byte('\x0c'), std::byte('\x0d'), std::byte('\x20')};
const std::vector<std::vector<std::byte>> WHITESPACES_D = {
	{std::byte('\xc2'), std::byte('\x85')},
	{std::byte('\xc2'), std::byte('\xa0')}
};
const std::vector<std::vector<std::byte>> WHITESPACES_T = {
	{std::byte('\xe1'), std::byte('\x9a'), std::byte('\x80')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x81')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x82')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x83')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x84')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x85')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x86')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x87')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x88')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x89')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x8a')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\xa8')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\xa9')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\xaf')},
	{std::byte('\xe2'), std::byte('\x81'), std::byte('\x9f')},
	{std::byte('\xe3'), std::byte('\x80'), std::byte('\x80')},
	{std::byte('\xe1'), std::byte('\xa0'), std::byte('\x8e')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x8b')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x8c')},
	{std::byte('\xe2'), std::byte('\x80'), std::byte('\x8d')},
	{std::byte('\xe2'), std::byte('\x81'), std::byte('\xa0')},
	{std::byte('\xef'), std::byte('\xbb'), std::byte('\xbf')}
};

bool is_valid_continuation(std::byte previous_byte, std::byte current_byte);

inline bool is_valid(std::byte byte);

bool is_space(const std::vector<std::byte> &bytes);

size_t count_words(const std::vector<std::vector<std::byte>> &bytes, [[maybe_unused]] int order);

std::vector<std::vector<std::byte>> normalize(const std::vector<std::byte> &bytes);

std::vector<std::pair<std::byte, size_t>> validate(const std::vector<std::byte> &bytes);
}
#endif //UNICODEPROCESSOR_UTF8_H
