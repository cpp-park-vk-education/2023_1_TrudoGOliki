cmake_minimum_required(VERSION 3.20)
project(file_system)

add_library(file_system src/file_system.cpp)

add_subdirectory(lib/file)
add_subdirectory(lib/avl_tree)
add_subdirectory(lib/sha256)

target_include_directories(file_system PUBLIC include)
target_link_libraries(file_system avl_tree file sha256)