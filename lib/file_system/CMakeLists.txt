project(file_system VERSION 0.1.0)
set(HEADERS include/file_system.hpp include/manager_files_cli.hpp)
set(SOURCES src/file_system.cpp src/manager_files_cli.cpp)

add_library(file_system ${SOURCES} ${HEADERS})


set(LINK_LIBRARIES avl_tree file buffer sha256 constants errors)

set(SUB_DIR lib/constants lib/file lib/avl_tree lib/buffer lib/sha256 lib/errors)
foreach(subdir ${SUB_DIR})
  add_subdirectory(${subdir})
endforeach()

target_include_directories(file_system PUBLIC include)
target_link_libraries(file_system ${LINK_LIBRARIES})