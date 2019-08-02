#include "Noun.hpp"
#include "Server/Ansi.hpp"

using namespace Mud::World;

Noun::Noun(const std::string &name,
     std::vector<Mud::Dictionary::Token> &&referents)
    : m_name(name), m_isUser(false),
      m_here(nullptr),
      m_indefiniteName(std::string("a ") + name),
      m_definiteName(std::string("the ") + name),
      m_capDefiniteName(std::string("The ") + name),
      m_description(std::string("There's nothing special about the ") + name),
      m_referents(std::move(referents))
//      m_luaRef(LUA_REFNIL)
{}

void Noun::RemoveItem(Noun &item)
{
    auto end = m_itemsHere.end(),
        pos = std::find(m_itemsHere.begin(),
                        end, &item);

    if (pos != end)
    {
        *pos = *(end-1);
        m_itemsHere.pop_back();
    }
}

Noun *Noun::GetExit(Direction dir) const
{
    auto it = m_exits.find(dir);
    if (it != m_exits.end()) return it->second;
    return nullptr;
}

std::ostream &operator<<(std::ostream &stream, const Noun &noun)
{
    return stream << BOLDTEXT << noun.Name() << PLAINTEXT NEWLINE
                  << noun.Description() << NEWLINE;
}
