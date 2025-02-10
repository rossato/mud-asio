#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <type_traits>

#include "Dictionary/Tokenizer.hpp"
#include "BasicMatchers.hpp"
#include "TokenRule.hpp"

namespace Mud
{
namespace Parser
{

template <class ContextType> class GrammarBuilder;
    
template <class ContextType>
class GrammarBase
{
public:
    virtual ~GrammarBase() = default;
    virtual bool TryParse(ContextType &c, Dictionary::Tokenizer &tok) const = 0;

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
// Would love to use a function template instead of static-casts,
//  but function templates and virtual functions don't mix.
//
template <class ActionType, class ContextType>
class Grammar : public GrammarBase<ContextType>
{
    friend class GrammarBuilder<ContextType>;
    
    typedef typename ActionType::DirectMatcher   DirectMatcher;
    typedef typename ActionType::IndirectMatcher IndirectMatcher;

public:
    virtual bool TryParse(ContextType &c, Dictionary::Tokenizer &tokenizer) const override
    {
        typename DirectMatcher::ValueType direct;
        typename IndirectMatcher::ValueType indirect;
        
        std::vector<TokenRule>::const_iterator rule, end = this->m_rules.end();

        for (rule = this->m_rules.begin(); rule != end;)
        {
            Dictionary::Token token;        
            bool found = false;

            switch (rule->tokenType)
            {
            case TokenType::GRAMMAR:
                token = tokenizer.GetToken();
                for (; rule != end && rule->tokenType == TokenType::GRAMMAR; ++rule)
                    if (rule->token == token) found = true;

                if (!found) return false;
                continue;

            case TokenType::DIRECT:
                if (!(direct = DirectMatcher::Match(c, tokenizer)))
                {
                    return false;
                }
                break;

            case TokenType::INDIRECT:
                if (!(indirect = IndirectMatcher::Match(c, tokenizer)))
                {
                    return false;
                }
                break;
            }
            ++rule;
        }
        if (tokenizer) return false;

        if constexpr (std::is_same_v<DirectMatcher, NoneMatcher>) {
            ActionType::Act(c);
        }
        else if constexpr (std::is_same_v<IndirectMatcher, NoneMatcher>) {
            ActionType::Act(c, direct);
        }
        else {
            ActionType::Act(c, direct, indirect);
        }
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
