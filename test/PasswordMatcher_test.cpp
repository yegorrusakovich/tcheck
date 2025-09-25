// TODO RYS fix dependencies, for test only, build as lib
// TODO RYS add src as include directory
// TOOD RYS compare gtest with catch2 and doctest
#include "../src/matchers.hpp"

#include <gtest/gtest.h>

TEST(Validation, Base)
{
    tcheck::PasswordMatcher matcher;
    ASSERT_TRUE(matcher.IsValid("rtG3FG!Tr^e"));
    ASSERT_FALSE(matcher.IsValid("password"));
}

TEST(Validation, FailConditions)
{
    tcheck::PasswordMatcher matcher;
    EXPECT_FALSE(matcher.IsValid("rtG3FG!Tr^eя")) << "only latin";
    EXPECT_FALSE(matcher.IsValid("rtG3FG!Tr^e.")) << "only difined special symbols";
    EXPECT_FALSE(matcher.IsValid("rt3FG!T")) << "at least 8";
    EXPECT_FALSE(matcher.IsValid("pas12!#ln")) << "at least one A-Z";
    EXPECT_FALSE(matcher.IsValid("PAS12!#LN")) << "at least one a-z";
    EXPECT_FALSE(matcher.IsValid("PASjf!#LN")) << "at least one digit";
    EXPECT_FALSE(matcher.IsValid("PASjf!!4N")) << "at least two different special symbols";
    EXPECT_FALSE(matcher.IsValid("rrtG3FG!Tr^e")) << "no two identical symbols in a row";
}

TEST(Validation, ComplexTrueCases)
{
    tcheck::PasswordMatcher matcher;
    ASSERT_TRUE(matcher.IsValid("rtG3FG!Tr^e"));
    ASSERT_TRUE(matcher.IsValid("aA1!*!1Aa"));
    ASSERT_TRUE(matcher.IsValid("enroi#$rkdeR#$092uwedchf34tguv394h"));
    ASSERT_FALSE(matcher.IsValid("aA1!b!1Aa"));
}
