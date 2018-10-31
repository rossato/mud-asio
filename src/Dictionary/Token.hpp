#ifndef TOKEN_HPP
#define TOKEN_HPP

namespace Mud
{
namespace Dictionary
{

enum TokenCategory : short
{
    NOWORD = 0,
    ARTICLE,
    SELF,
    DIRECTION,
    GRAMMAR,
    NOUN
};

class Token
{
public:
    // Hopefully fits into a register
    short index;
    TokenCategory type;

    constexpr bool operator==(const Token &other) const
    {
        return index == other.index;
    }
    explicit constexpr operator bool() const { return index; }

    constexpr bool IsArticle() const
        { return type == ARTICLE; }
    constexpr bool IsSelf() const
        { return type == SELF; }
    constexpr bool IsDirection() const
        { return type == DIRECTION; }
    // Directions are also sometimes tokens of grammar (up, down, in, out)
    //  so this might need to get widened to grammar | direction
    constexpr bool IsGrammar() const
        { return type == GRAMMAR; }
    constexpr bool IsNoun() const
        { return type == NOUN; }

    constexpr TokenCategory GetCategory() const
        { return type; }
    constexpr short GetRawIndex() const
        { return index; }
};

}
}

#endif
