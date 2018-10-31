#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include "Dictionary.hpp"
#include "ItReference.hpp"
#include "Token.hpp"

namespace Mud
{
namespace Dictionary
{

// Has std::istringstream semantics, sort of.

class Tokenizer : public ItReference {
public:
    explicit Tokenizer(Dictionary &dict)
        : m_dictionary(dict)
    {
        m_cache.reserve(5);
    }

    void Str(const std::string &line);

    Token              GetToken();
    const std::string &GetString();

    explicit operator bool() const {
        return (m_pos != m_end) ||
            (m_cacheIterator != m_cache.end());
    }

    int Tell() const
        { return m_cacheIterator - m_cache.begin(); }

    void Seek(int pos)
        { m_cacheIterator = m_cache.begin() + pos; }

    void Unget()
        { --m_cacheIterator; }

    std::string DumpRestOfLine();

private:
    void IgnoreTelnetCommand();
    void AdvanceToNextToken();
    const std::string &ReadNextString();

    Dictionary &m_dictionary;

    using CacheType = std::vector<std::pair<std::string, Token> >;
    CacheType m_cache;
    CacheType::iterator m_cacheIterator;

    std::string::const_iterator m_pos, m_end;
};

}
}

#endif
