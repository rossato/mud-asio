#ifndef NOUN_HPP
#define NOUN_HPP

#include <algorithm>
#include <string>
#include <vector>
#include "Dictionary/Token.hpp"

namespace Mud
{
namespace World
{

// This class represents something a verb can match with NounMatcher.
// This is basically Users, Locations, and Items.

class Noun
{
    friend class NounMatcher;

public:
    // Non-proper noun
    Noun(const std::string &name,
         std::vector<Dictionary::Token> &&referents)
        : m_name(name), m_user(false),
          m_indefiniteName(std::string("a ") + name),
          m_definiteName(std::string("the ") + name),
          m_capDefiniteName(std::string("The ") + name),
          m_referents(std::move(referents))
    {
    }

    Noun &WithIndefiniteName(const std::string &indefinite)
        { m_indefiniteName = indefinite; return *this; }
    Noun &WithDefiniteName(const std::string &definite)
        { m_definiteName = definite; return *this; }
    Noun &WithCapitalizedDefiniteName(const std::string &capDefinite)
        { m_capDefiniteName = capDefinite; return *this; }

    const std::string &Name() const { return m_name; }
    const std::string &aName() const { return m_indefiniteName; }
    const std::string &theName() const { return m_definiteName; }
    const std::string &TheName() const { return m_capDefiniteName; }
    
    bool IsReferredToAs(Dictionary::Token referent)
    {
        return std::find(m_referents.begin(), m_referents.end(), referent)
            != m_referents.end();
    }

    bool IsAUser() const { return m_user; }
    
protected:
    std::string m_name;
    bool m_user;

private:
    std::string m_indefiniteName, m_definiteName, m_capDefiniteName;
    std::vector<Dictionary::Token> m_referents;
};

}
}

#endif
