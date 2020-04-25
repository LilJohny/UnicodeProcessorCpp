cmake_minimum_required(VERSION 3.16)
project(UnicodeProcessor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
add_executable(UnicodeProcessor main.cpp)