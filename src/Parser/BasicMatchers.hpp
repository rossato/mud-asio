#ifndef BASIC_MATCHERS_HPP
#define BASIC_MATCHERS_HPP

#include <optional>
#include <string>
#include "Dictionary/Tokenizer.hpp"

namespace Mud
{
namespace Parser
{
    
struct NoneMatcher
{
    static const std::string Description;

    typedef int ValueType;

    template <class ContextType>
    static constexpr ValueType Match(ContextType &, Dictionary::Tokenizer &) {return 0;}
};

struct NumberMatcher
{
    static const std::string Description;

    typedef std::optional<int> ValueType;

    template <class ContextType>
    static ValueType Match(ContextType &, Dictionary::Tokenizer &tokenizer)
    {
        // Dictionary::Token token = tok.GetNextToken();

        // if (token.IsNumber())
        //     return token.GetRawIndex();

        // return ValueType();

        try
        {
            return std::stoi(tokenizer.GetString());
        }
        catch(const std::exception &)
        {
            return ValueType();
        }
    }
};

struct VerbMatcher
{
    static const std::string Description;
    
    typedef const std::string* ValueType;

    template <class ContextType>
    static ValueType Match(ContextType &, Dictionary::Tokenizer &tokenizer)
    {
        auto &verb = tokenizer.GetString();
        if (verb.empty()) return nullptr;
        return &verb;
    }
};

struct RestOfLineMatcher
{
    static const std::string Description;

    typedef std::optional<std::string> ValueType;

    template <class ContextType>
    static ValueType Match(ContextType &, Dictionary::Tokenizer &tokenizer)
    {
        return tokenizer.DumpRestOfLine();
    }
};

}
}

#endif
