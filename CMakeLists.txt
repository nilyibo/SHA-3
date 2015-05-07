cmake_minimum_required (VERSION 2.6)
project (SHA3)

add_executable (test123 main.cpp)

add_library (SHA3 keccak.cpp)

target_link_libraries (test123 SHA3)
