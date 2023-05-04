add_library(cli src/cli.cpp)
target_include_directories(cli PUBLIC include)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()