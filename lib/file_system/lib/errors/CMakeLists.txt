set(SOURCES src/fs_error.cpp)

add_library(errors ${SOURCES})


target_include_directories(errors PUBLIC include)