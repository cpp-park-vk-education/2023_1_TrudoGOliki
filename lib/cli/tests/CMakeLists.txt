add_executable(cli_test src/test_cli.cpp)
target_link_libraries(cli_test cli GTest::gtest_main)

add_test(cli_test cli_test)