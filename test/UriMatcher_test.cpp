#include <gtest/gtest.h>

#include <mtch/matchers.hpp>

TEST(UriValidation, Base) {
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

TEST(UriValidation, Components) {
  tcheck::UriMatcher matcher;

  // ipv4
  EXPECT_TRUE(matcher.IsValid("http://192.168.1.7:21"));
  EXPECT_FALSE(matcher.IsValid("http://256.168.1.7:21"));
  EXPECT_FALSE(matcher.IsValid("http://255.168.1.7.8:21"));
  EXPECT_FALSE(matcher.IsValid("http://255.168.1.78989:21"));
}

TEST(UriValidation, TrickyCases) {
  tcheck::UriMatcher matcher;

  EXPECT_TRUE(matcher.IsValid("ftp://host:21/path/to%20file"))
      << "percantage encoding";

  EXPECT_TRUE(
      matcher.IsValid("http://[2001:db8::1]:8080/?query=complex&value=1"))
      << "ipv6 and query";

  EXPECT_TRUE(matcher.IsValid("http://host?")) << "empty query";
}

/// @brief Idea here to move semantic obligations to regexp too
/// @attention it maybe bad decision check semantic by tool for syntax checks
TEST(UriValidation, Http) {
  tcheck::UriMatcher matcher;
  // TODO RYS here print GetUserErrorDescription

  EXPECT_FALSE(matcher.IsValid("http://example.com:0"))
      << "port: invalid range";
  EXPECT_FALSE(matcher.IsValid("https://example.com:65536"))
      << "port: invalid range";
  EXPECT_FALSE(matcher.IsValid("http://example.com:70000"))
      << "port: invalid range";

  EXPECT_FALSE(matcher.IsValid("http://:8080/path")) << "authority: no host";
  EXPECT_FALSE(matcher.IsValid("http:///path")) << "authority: no host";
  EXPECT_FALSE(matcher.IsValid("http://")) << "authority: no authority";
}
