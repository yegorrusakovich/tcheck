#include "../src/matchers.hpp"

#include <gtest/gtest.h>

TEST(Validation, Base)
{
    tcheck::UriMatcher matcher;
    
    EXPECT_TRUE(matcher.IsValid("http://example.com"));
    EXPECT_FALSE(matcher.IsValid("httttttp://example.com"));

    EXPECT_TRUE(matcher.IsValid("https://ex.com/articles/page1?v=2&s=ex"));
    EXPECT_FALSE(matcher.IsValid("http//example.com"));

    EXPECT_TRUE(matcher.IsValid("http://127.0.0.1:1235"));
    EXPECT_FALSE(matcher.IsValid("C://Users/User/example.com"));

    EXPECT_TRUE(matcher.IsValid("ftp://example.com/files/example.txt"));
    EXPECT_FALSE(matcher.IsValid("/home/user/example.com"));

    EXPECT_TRUE(matcher.IsValid("http://xn--fsqu00a.xn--3lr804guic/"));
    EXPECT_TRUE(matcher.IsValid("ssh://login@server.com:1234/repository.git"));
    EXPECT_TRUE(matcher.IsValid("smb://192.168.1.7/USER$/"));
    EXPECT_TRUE(matcher.IsValid("ftp://ftp.example.com/path/"));
}
