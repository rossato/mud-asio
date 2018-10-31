#ifndef GRAMMAR_LINE_HPP
#define GRAMMAR_LINE_HPP

#include "Dictionary/Tokenizer.hpp"
#include "TokenRule.hpp"

namespace Mud
{
namespace World
{
class User;
}
namespace Grammar
{

class GrammarLineBase
{
public:
    virtual ~GrammarLineBase() = default;
    virtual bool TryParse(World::User &user, std::ostream &response, Dictionary::Tokenizer &tok) const = 0;

    template <class T>
    void EmplaceRule(T &&rule)
    {
        m_rules.emplace_back(std::forward<T>(rule));
    }

    virtual const std::string &DirectDescription() const = 0;
    virtual const std::string &IndirectDescription() const = 0;

protected:
    std::vector<TokenRule> m_rules;
};

////////////////////////////////////////////////////////
//
// This class synthesizes multiple pluggable strategies
//  into a single virtual parse call.
//
template <class ActionType>
class GrammarLine : public GrammarLineBase
{
    typedef typename ActionType::DirectMatcher   DirectMatcher;
    typedef typename ActionType::IndirectMatcher IndirectMatcher;

public:
    virtual bool TryParse(World::User &user, std::ostream &response,
                          Dictionary::Tokenizer &tok) const override
    {
        typename DirectMatcher::ValueType direct;
        typename IndirectMatcher::ValueType indirect;

        Dictionary::Token token;

        std::vector<TokenRule>::const_iterator rule, end = m_rules.end();

        for (rule = m_rules.begin(); rule != end;)
        {
            bool found = false;
            switch (rule->tokenType)
            {
            case TokenType::GRAMMAR:
                token = tok.GetToken();
                for (; rule != end && rule->tokenType == TokenType::GRAMMAR; ++rule)
                    if (rule->token == token) found = true;

                if (!found) return false;
                continue;

            case TokenType::DIRECT:
                if (!(direct = DirectMatcher::Match(user, tok))) return false;
                break;

            case TokenType::INDIRECT:
                if (!(indirect = IndirectMatcher::Match(user, tok))) return false;
                break;
            }
            ++rule;
        }
        if (tok) return false;

        ActionType::Act(user, response, direct, indirect);
        return true;
    }

    virtual const std::string &DirectDescription() const override
    {
        return DirectMatcher::Description;
    }

    virtual const std::string &IndirectDescription() const override
    {
        return IndirectMatcher::Description;
    }
};

}
}

#endif
