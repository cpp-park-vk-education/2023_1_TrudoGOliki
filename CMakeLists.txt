cmake_minimum_required(VERSION 3.20)
project(Vkyrse_all VERSION 0.1.0)

add_subdirectory(lib/cli)
add_subdirectory(lib/network network)
add_subdirectory(lib/file_system)
add_subdirectory(lib/protocol)

add_subdirectory(src_cli)
add_subdirectory(src_deamon)