cmake_minimum_required(VERSION 3.20)
project(CLI_Lib)

set(SOURCES src/cli.cpp src/Option.cpp src/cli_cmds/lookfile.cpp src/cli_cmds/sharefile.cpp src/cli_cmds/status.cpp src/cli_cmds/hidefile.cpp src/cli_cmds/help.cpp)
set(HEADERS include/cli.hpp include/Option.hpp include/IExecutable.hpp)

#set(SOURCES ./src)
#set(HEADERS ./include)

add_compile_options(-Wall -Wextra)




#add_library(cli )
#target_include_directories(cli PUBLIC include)

if(TARGET cli)

message("cli is already defined")

else()

add_library(cli ${SOURCES} ${HEADERS})
target_link_libraries(cli protocol)
target_include_directories(cli PUBLIC ./include)

endif()


if(BUILD_TESTS)
  add_subdirectory(tests)
endif()