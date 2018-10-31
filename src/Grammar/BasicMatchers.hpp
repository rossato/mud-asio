#ifndef BASIC_MATCHERS_HPP
#define BASIC_MATCHERS_HPP

#include <optional>
#include <string>
#include "Dictionary/Tokenizer.hpp"

namespace Mud
{
namespace World
{
    class User;
}
namespace Grammar
{
    
struct NoneMatcher
{
    static const std::string Description;

    typedef int ValueType;

    static constexpr ValueType Match(World::User &, Dictionary::Tokenizer &) {return 0;}
};

struct NumberMatcher
{
    static const std::string Description;

    typedef std::optional<int> ValueType;

    static ValueType Match(World::User &, Dictionary::Tokenizer &tok)
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

    static ValueType Match(World::User &, Dictionary::Tokenizer &tok)
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

    static ValueType Match(World::User &, Dictionary::Tokenizer &tok)
    {
        return tok.DumpRestOfLine();
    }
};

}
}

#endif
