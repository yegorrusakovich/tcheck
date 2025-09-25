// TODO RYS fix dependencies, for test only, build as lib
// TODO RYS add src as include directory
// TOOD RYS compare gtest with catch2 and doctest
#include "../src/password_search.hpp"

#include <gtest/gtest.h>

TEST(Validation, Base)
{
    tcheck::PasswordProcessor processor;
    ASSERT_TRUE(processor.IsValid("rtG3FG!Tr^e"));
    ASSERT_FALSE(processor.IsValid("password"));
}

TEST(Validation, FailConditions)
{
    tcheck::PasswordProcessor processor;
    EXPECT_FALSE(processor.IsValid("rtG3FG!Tr^eя")) << "only latin";
    EXPECT_FALSE(processor.IsValid("rtG3FG!Tr^e.")) << "only difined special symbols";
    EXPECT_FALSE(processor.IsValid("rt3FG!T")) << "at least 8";
    EXPECT_FALSE(processor.IsValid("pas12!#ln")) << "at least one A-Z";
    EXPECT_FALSE(processor.IsValid("PAS12!#LN")) << "at least one a-z";
    EXPECT_FALSE(processor.IsValid("PASjf!#LN")) << "at least one digit";
    EXPECT_FALSE(processor.IsValid("PASjf!!4N")) << "at least two different special symbols";
    EXPECT_FALSE(processor.IsValid("rrtG3FG!Tr^e")) << "no two identical symbols in a row";
}

TEST(Validation, ComplexTrueCases)
{
    tcheck::PasswordProcessor processor;
    ASSERT_TRUE(processor.IsValid("rtG3FG!Tr^e"));
    ASSERT_TRUE(processor.IsValid("aA1!*!1Aa"));
    ASSERT_TRUE(processor.IsValid("enroi#$rkdeR#$092uwedchf34tguv394h"));
    ASSERT_FALSE(processor.IsValid("aA1!b!1Aa"));
}
