cmake_minimum_required(VERSION 3.14)
project(pagerank)


set(CMAKE_CXX_STANDARD 11)
add_compile_options(-O3  -Wno-unused-result)
add_executable(pagerank main.c)
