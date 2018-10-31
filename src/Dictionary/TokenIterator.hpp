#ifndef TOKEN_ITERATOR_HPP
#define TOKEN_ITERATOR_HPP

namespace Mud
{
namespace Dictionary
{

class TokenIterator
{
public:
    TokenIterator(Tokenizer &tok, Tokenizer::CacheType::const_iterator start)
        : m_tokenizer(tok), m_pos(start) {}

    TokenIterator &operator++()
    {
        ++m_pos;
        if (m_pos == m_tokenizer.m_tokenCache.end())
        {
            m_tokenizer.ReadNextToken();
            m_pos = m_tokenizer.m_tokenCache.end()-1;
        }
    }

    Token operator*() const
    {
        return *m_pos;
    }
                                        }
private:
    Tokenizer &m_tokenizer;
    Tokenizer::CacheType::const_iterator m_pos;
};
    
}   
}

#endif
