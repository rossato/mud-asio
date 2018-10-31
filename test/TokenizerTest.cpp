#include <gtest/gtest.h>
#include "Dictionary/Dictionary.hpp"
#include "Dictionary/Tokenizer.hpp"

using namespace Mud::Dictionary;

namespace Mud
{
namespace Test
{

class TokenizerTest : public ::testing::Test
{
protected:
    TokenizerTest() : tok(dictionary) {}
    
    Dictionary::Dictionary dictionary;
    Tokenizer tok;
};

TEST_F(TokenizerTest, SingleWordLine)
{
    tok.Str("line");
    EXPECT_EQ(tok.GetString(), "line");
}

TEST_F(TokenizerTest, SingleWordTrailingSpace)
{
    tok.Str("line ");
    EXPECT_EQ(tok.GetString(), "line");
}

TEST_F(TokenizerTest, SingleWordTrailingCR)
{
    tok.Str("line\r");
    EXPECT_EQ(tok.GetString(), "line");
}

TEST_F(TokenizerTest, OneToken)
{
    tok.Str("do it");
    tok.GetString();
    EXPECT_EQ(tok.GetToken(), Dictionary::Dictionary::It);
}

}
}
