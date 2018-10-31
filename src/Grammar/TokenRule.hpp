#ifndef TOKEN_RULE_HPP
#define TOKEN_RULE_HPP

#include "Dictionary/Token.hpp"

namespace Mud
{
namespace Grammar
{

enum class TokenType {
    GRAMMAR,
    DIRECT,
    INDIRECT
};

class TokenRule
{
public:
    TokenRule(Dictionary::Token word)
        : tokenType(TokenType::GRAMMAR), token(word) {}

    TokenRule(TokenType type) : tokenType(type) {}

    TokenType tokenType;
    Dictionary::Token token;
};

}
}

#endif
