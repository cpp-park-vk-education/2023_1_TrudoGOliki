set(SOURCES src/standard_buffer_size.cpp src/main_dir.cpp)

add_library(constants ${SOURCES})

target_include_directories(constants PUBLIC include)