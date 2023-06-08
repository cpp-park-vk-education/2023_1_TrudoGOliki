cmake_minimum_required(VERSION 3.20)
project(file_system)


if(TARGET file_system)

message("file_system is already defined")

else()

add_library(file_system src/file_system.cpp)

add_subdirectory(lib/file)
add_subdirectory(lib/avl_tree)
add_subdirectory(lib/buffer)
add_subdirectory(lib/sha256)

target_include_directories(file_system PUBLIC include)

target_link_libraries(file_system avl_tree file buffer sha256)
endif()

