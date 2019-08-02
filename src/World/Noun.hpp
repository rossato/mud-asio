#ifndef NOUN_HPP
#define NOUN_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Dictionary/Token.hpp"
#include "Location.hpp"

namespace Mud
{
namespace World
{
    
// This class represents something a verb can match with NounMatcher.
// This is basically Users, Locations, and Items.

class Noun : public Location
{
    friend class NounMatcher;
    friend class World;
public:
    // Non-proper noun
    Noun(const std::string &name,
         std::vector<Dictionary::Token> &&referents);

    Noun &WithIndefiniteName(const std::string &indefinite)
        { m_indefiniteName = indefinite; return *this; }
    Noun &WithDefiniteName(const std::string &definite)
        { m_definiteName = definite; return *this; }
    Noun &WithCapitalizedDefiniteName(const std::string &capDefinite)
        { m_capDefiniteName = capDefinite; return *this; }
    Noun &WithDescription(const std::string &description)
        { m_description = description; return *this; }

    const std::string &Name() const { return m_name; }
    const std::string &aName() const { return m_indefiniteName; }
    const std::string &theName() const { return m_definiteName; }
    const std::string &TheName() const { return m_capDefiniteName; }
    const std::string &Description() const { return m_description; }
    
    bool IsReferredToAs(Dictionary::Token referent)
    {
        return std::find(m_referents.begin(), m_referents.end(), referent)
            != m_referents.end();
    }

    bool IsAUser() const { return m_isUser; }

    const std::vector<Noun*> &GetItems() const { return m_itemsHere; }
    void RemoveItem(Noun &);
    void AddItem(Noun &noun)
        { m_itemsHere.emplace_back(&noun); }

    Noun &SetExit(Direction dir, int otherLocation)
    {
        m_exitIndexes.emplace_back(dir, otherLocation);
        return *this;
    }
    Noun *GetExit(Direction dir) const;

    Noun *GetLocation() { return m_here; }
    void SetLocation(Noun *noun) { m_here = noun; }

    // void SetLuaRef(int ref) { m_luaRef = ref; }
    // int LuaRef() const { return m_luaRef; }
    
protected:
    std::vector<Noun*> m_itemsHere;
    std::string m_name;
    bool m_isUser;
    Noun *m_here;

private:
    std::vector<std::pair<Direction, int> > m_exitIndexes;
    std::map<Direction, Noun*> m_exits;

    std::string m_indefiniteName, m_definiteName, m_capDefiniteName, m_description;
    std::vector<Dictionary::Token> m_referents;
    // int m_luaRef;
};

}
}

std::ostream &operator<<(std::ostream &stream, const Mud::World::Noun &noun);

#endif
