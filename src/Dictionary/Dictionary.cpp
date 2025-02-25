#include "Dictionary.hpp"

#include <algorithm>
#include <iostream>

using namespace Mud::Dictionary;

Dictionary::Dictionary()
{
    std::cout << "Initializing base vocabulary..." << std::endl;

    Insert("", NOWORD);
    Insert("it", NOUN);

    // articles (3)
    Insert("a",   ARTICLE);
    Insert("an",  ARTICLE);
    Insert("the", ARTICLE);

    // self (4)
    Insert("me",     SELF);
    Insert("myself", SELF);
    Insert("I",      SELF);
    Insert("self",   SELF);

    // directions (22)
    Insert("n",         DIRECTION);
    Insert("north",     DIRECTION);
    Insert("e",         DIRECTION);
    Insert("east",      DIRECTION);
    Insert("s",         DIRECTION);
    Insert("south",     DIRECTION);
    Insert("w",         DIRECTION);
    Insert("west",      DIRECTION);
    Insert("ne",        DIRECTION);
    Insert("northeast", DIRECTION);
    Insert("se",        DIRECTION);
    Insert("southeast", DIRECTION);
    Insert("sw",        DIRECTION);
    Insert("southwest", DIRECTION);
    Insert("nw",        DIRECTION);
    Insert("northwest", DIRECTION);
    Insert("u",         DIRECTION);
    Insert("up",        DIRECTION);
    Insert("d",         DIRECTION);
    Insert("down",      DIRECTION);
    Insert("in",        DIRECTION);
    Insert("out",       DIRECTION);
}

Dictionary::IndexType Dictionary::TryLookUp(const Dictionary::WordType &word) const {
    // Parse as word
    const auto cit = m_dictionary.find(word);
    if (cit != m_dictionary.end())
        return cit->second;

    // Parse as number
    // try
    // {
    //     return Token {
    //         index: static_cast<short>(std::stoi(word)),
    //         type: NUMBER
    //     };
    // }
    // catch (const std::exception &)
    // {
        // We could return NoWord instead, but grammars that accept an unknown word
        //  will be rare, much faster to abort processing immediately.
        throw WordNotFoundException(word);
    // }
}

Dictionary::IndexType Dictionary::LookUp(const Dictionary::WordType &word) const {
    // Parse as word
    const auto cit = m_dictionary.find(word);
    if (cit != m_dictionary.end())
        return cit->second;

    return NoWord;
}

Dictionary::IndexType Dictionary::Insert(Dictionary::WordType word,
                                         TokenCategory category) {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    
    const auto it = m_dictionary.find(word);
    if (it != m_dictionary.end()) {
        return it->second;
    }
    IndexType token = {index: static_cast<short>(m_dictionary.size()),
//static_cast<short>(m_words.size()),
                       type: category};
//    m_words.emplace_back(word);
    return m_dictionary[word] = token;
}
