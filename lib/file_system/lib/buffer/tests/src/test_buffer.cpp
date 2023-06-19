#include <gtest/gtest.h>

#include <string>

#include "buffer.hpp"

bool isBufEqual(const char *buf1, const char *buf2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) {
            return false;
        }
    };

    return true;
}

TEST(BufferTest, ExceptionInitZero) {
    EXPECT_THROW(buf::Buffer(0), buf::BufError);
}

TEST(BufferTest, BufNullptrInitAttempt) {
    EXPECT_THROW(buf::Buffer(nullptr, 10), buf::BufError);
}

TEST(BufferTest, BasicInit) {
    size_t size = 10;
    EXPECT_NO_THROW(auto buffer = buf::Buffer(size);
                    EXPECT_NE(buffer.buf_, nullptr);
                    EXPECT_EQ(buffer.size_, size););
}

TEST(BufferTest, StringInit) {
    std::string str = "adsfsd";
    EXPECT_NO_THROW(
        auto buffer = buf::Buffer(str.c_str(), str.size());
        ASSERT_EQ(str.size(), buffer.size_);
        EXPECT_TRUE(isBufEqual(str.c_str(), buffer.get(), str.size())););
}

TEST(BufferTest, EmptyStringInit) {
    std::string str = "";
    EXPECT_THROW(buf::Buffer(str.c_str(), str.size()), buf::BufError);
}

TEST(BufferTest, OperatorPlus) {
    std::string str = "asd";
    std::string str2 = "TTT";
    EXPECT_NO_THROW(
        auto buf1 = buf::Buffer(str.c_str(), str.size());
        auto buf2 = buf::Buffer(str2.c_str(), str2.size());
        buf1 = buf1 + buf2;
        ASSERT_EQ(buf1.size_, (str.size() + str2.size()) );
        EXPECT_TRUE(isBufEqual(buf1.get(), (str+str2).c_str(), buf1.size_)));
}

TEST(BufferTest, BrokenBufferAssigment) {
    std::string str = "asd";
    std::string str2 = "TTT";
    EXPECT_NO_THROW(
        auto buf1 = buf::Buffer(str.c_str(), str.size());
        auto buf2 = buf::Buffer(str2.c_str(), str2.size());
        buf2.size_ = 0;
        EXPECT_THROW(buf1 = buf2, buf::BufError););
}

TEST(BufferTest, CopyAssignments) {
    std::string str = "asd";
    std::string str2 = "TTT";
    EXPECT_NO_THROW(
        auto buf1 = buf::Buffer(str.c_str(), str.size());
        auto buf2 = buf::Buffer(str2.c_str(), str2.size());
        auto buf3 = buf2;
        buf2 = buf3;
        buf1 = buf3;
        EXPECT_EQ(buf1, buf2);
        EXPECT_EQ(buf2, buf3);
    );
}

TEST(BufferTest, MoveAssignments) {
    std::string str = "asd";
    std::string str2 = "TTT";
    std::string str3 = "DDT";
    EXPECT_NO_THROW(
        auto buf1 = buf::Buffer(str.c_str(), str.size());
        auto buf2 = buf::Buffer(str2.c_str(), str2.size());
        auto buf3 = buf::Buffer(str3.c_str(), str3.size());
        buf2 = std::move(buf3);
        buf1 = std::move(buf2);
        EXPECT_EQ(buf2.size_, 0);
        EXPECT_EQ(buf3.size_, 0);
        EXPECT_EQ(buf2.buf_, nullptr);
        EXPECT_EQ(buf3.buf_, nullptr);
        ASSERT_EQ(buf1.size_, str3.size());
        EXPECT_TRUE(isBufEqual(buf1.get(), str3.c_str(), str3.size())););
}

TEST(BufferTest, MoveConstructor) {
    std::string str = "asd";
    EXPECT_NO_THROW(
        auto buf1 = buf::Buffer(str.c_str(), str.size());
        auto buf2 = std::move(buf1);
        EXPECT_EQ(buf1.size_, 0);
        EXPECT_EQ(buf1.buf_, nullptr);
        ASSERT_EQ(buf2.size_, str.size());
        EXPECT_TRUE(isBufEqual(buf2.get(), str.c_str(), str.size())););
}

