#ifndef BASIC_MATCHERS_HPP
#define BASIC_MATCHERS_HPP

#include <optional>
#include <string>
#include "Dictionary/Tokenizer.hpp"

namespace Mud
{
namespace Grammar
{
    
struct NoneMatcher
{
    static const std::string Description;

    typedef int ValueType;
    typedef Dictionary::Tokenizer InterfaceType;
    
    static constexpr ValueType Match(InterfaceType &) {return 0;}
};

struct NumberMatcher
{
    static const std::string Description;

    typedef std::optional<int> ValueType;
    typedef Dictionary::Tokenizer InterfaceType;

    static ValueType Match(InterfaceType &tok)
    {
        // Dictionary::Token token = tok.GetNextToken();

        // if (token.IsNumber())
        //     return token.GetRawIndex();

        // return ValueType();

        try
        {
            return std::stoi(tok.GetString());
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
    typedef Dictionary::Tokenizer InterfaceType;

    static ValueType Match(InterfaceType &tok)
    {
        auto &verb = tok.GetString();
        if (verb.empty()) return nullptr;
        return &verb;
    }
};

struct RestOfLineMatcher
{
    static const std::string Description;

    typedef std::optional<std::string> ValueType;
    typedef Dictionary::Tokenizer InterfaceType;

    static ValueType Match(InterfaceType &tok)
    {
        return tok.DumpRestOfLine();
    }
};

}
}

#endif
