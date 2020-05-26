cmake_minimum_required(VERSION 3.16)
project(UnicodeProcessor)

set(CMAKE_CXX_STANDARD 17)
add_executable(UnicodeProcessor main.cpp utf8.h utf16.h utf32.h utf8.cpp utf16.cpp utf32.cpp unicode.h unicode.cpp)