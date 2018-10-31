#include "Dictionary.hpp"

#include <algorithm>
#include <iostream>

using namespace Mud::Dictionary;

Dictionary::Dictionary()
{
    std::cout << "Initializing base vocabulary..." << std::endl;

    TryInsert("", NOWORD);
    TryInsert("it", NOUN);

    // articles (3)
    TryInsert("a",   ARTICLE);
    TryInsert("an",  ARTICLE);
    TryInsert("the", ARTICLE);

    // self (4)
    TryInsert("me",     SELF);
    TryInsert("myself", SELF);
    TryInsert("I",      SELF);
    TryInsert("self",   SELF);

    // directions (22)
    TryInsert("n",         DIRECTION);
    TryInsert("north",     DIRECTION);
    TryInsert("e",         DIRECTION);
    TryInsert("east",      DIRECTION);
    TryInsert("s",         DIRECTION);
    TryInsert("south",     DIRECTION);
    TryInsert("w",         DIRECTION);
    TryInsert("west",      DIRECTION);
    TryInsert("ne",        DIRECTION);
    TryInsert("northeast", DIRECTION);
    TryInsert("se",        DIRECTION);
    TryInsert("southeast", DIRECTION);
    TryInsert("sw",        DIRECTION);
    TryInsert("southwest", DIRECTION);
    TryInsert("nw",        DIRECTION);
    TryInsert("northwest", DIRECTION);
    TryInsert("u",         DIRECTION);
    TryInsert("up",        DIRECTION);
    TryInsert("d",         DIRECTION);
    TryInsert("down",      DIRECTION);
    TryInsert("in",        DIRECTION);
    TryInsert("out",       DIRECTION);
}

Dictionary::IndexType Dictionary::LookUp(const Dictionary::WordType &word) const {
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

Dictionary::IndexType Dictionary::TryInsert(Dictionary::WordType word,
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
