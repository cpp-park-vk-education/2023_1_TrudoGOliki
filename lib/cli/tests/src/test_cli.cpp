#include <gtest/gtest.h>
#include <string>

#include "cli.hpp"

TEST(ParseArgs, BasicCase) {
    std::string actual = parse_args(2, {"./Vkyrse", "-h"});
    std::string expected = "-h";
    EXPECT_EQ(expected, actual);
}