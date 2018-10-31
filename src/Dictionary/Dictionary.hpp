#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Dictionary/Token.hpp"

namespace Mud
{
namespace Dictionary
{

class WordNotFoundException : public std::exception {
public:
    WordNotFoundException(const std::string &word)
        : m_word(word), m_out("Dictionary error: Unrecognized word \"")
    {
        m_out << m_word << "\"";
    }

    constexpr const std::string &word() const { return m_word; }

    virtual const char *what() const noexcept override
    {
        return m_out.str().c_str();
    }
    
private:
    const std::string m_word;
    std::ostringstream m_out;
};

class Dictionary {
public:
    typedef Token IndexType;
    typedef std::string WordType;

    Dictionary();
    
    static constexpr IndexType NoWord = {0,NOWORD};
    static constexpr IndexType It     = {1,NOUN};
    static constexpr short DirectionOffset = 9;

    IndexType LookUp(const WordType &word) const;
    // const WordType &Words(IndexType index) const
    //     { return m_words[index.GetRawIndex()]; } // this might be an issue with NUMBER category
    IndexType TryInsert(WordType, TokenCategory);

private:
//    std::vector<WordType> m_words;
    std::map<WordType, IndexType> m_dictionary;
};

}
}

#endif
