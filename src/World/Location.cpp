#include "Location.hpp"

#include "Server/Ansi.hpp"
#include "Server/ConnectionBase.hpp"
#include "User.hpp"

namespace Mud
{
namespace World
{

void Location::UserArriving(User &user)
{
    std::ostringstream out;
    out << user.Name() << " arrives in the area." NEWLINE;
    std::string message = out.str();
    
    for (auto otherUser : m_usersHere)
    {
        otherUser->Broadcast(message);
    }

    m_usersHere.emplace_back(&user);
}

void Location::UserLeaving(User &user, Direction dir)
{
    auto pos = m_usersHere.begin(),
        end = m_usersHere.end(),
        userIt = end;

    std::ostringstream out;
    out << user.Name() << " leaves to " << dir << "." NEWLINE;
    std::string message = out.str();
    
    for (; pos != end; ++pos)
    {
        if (*pos == &user)
            userIt = pos;
        else
            (*pos)->Broadcast(message);
    }

    if (userIt != end)
    {
        *userIt = *(end-1);
        m_usersHere.pop_back();
    }
}

void Location::RemoveItem(Noun &item)
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

std::ostream &operator<<(std::ostream &stream, const Location &location)
{
    return stream << BOLDTEXT << location.m_name << PLAINTEXT NEWLINE
                  << location.m_description << NEWLINE;
}

}
}
