#include "Location.hpp"

#include "Server/Ansi.hpp"
#include "Server/ConnectionBase.hpp"
#include "User.hpp"

namespace Mud
{
namespace World
{

// void Location::Repop()
// {
//     m_itemsHere.clear();
//     for (auto &item : m_itemRepops)
//     {
//         m_itemsHere.emplace_back(&item);
//     }
// }

Location &Location::TellAllBut(User &actor, const std::string &message)
{
    for (auto user : m_usersHere)
    {
        if (user != &actor) user->Broadcast(message);
    }
    return *this;
}

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

}
}
