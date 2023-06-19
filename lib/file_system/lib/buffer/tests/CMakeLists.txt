add_executable(buffer_test src/test_buffer.cpp)
target_compile_options(buffer PUBLIC --coverage)
target_link_options(buffer PUBLIC --coverage)
target_link_libraries(buffer_test buffer GTest::gtest_main)

add_test(buffer_test buffer_test)