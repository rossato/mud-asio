#include "User.hpp"

#include "Dictionary/Dictionary.hpp"
#include "Location.hpp"
#include "World.hpp"

using namespace Mud::World;

User::User(Mud::World::World &world, const std::string &name, Location &startingPlace)
    : Noun(name, {world.Dictionary().TryInsert(name, Dictionary::NOUN)}),
      m_world(world), m_here(&startingPlace)
{
    WithIndefiniteName(name);
    WithDefiniteName(name);
    WithCapitalizedDefiniteName(name);
    m_user = true;
}


void User::RegisterConnection(Server::ConnectionBase &connection)
{
    if (m_connections.empty())
        m_here->UserArriving(*this);
    
    m_connections.emplace_back(&connection);
}

void User::DeregisterConnection(Server::ConnectionBase &connection)
{
    auto end = m_connections.end(),
        it = std::find(m_connections.begin(),
                       end, &connection);

    if (it != end)
    {
        *it = *(end-1);
        m_connections.pop_back();

        if (m_connections.empty())
            m_here->UserLeaving(*this, NODIR);
    }
}
    
void User::RemoveFromInventory(Noun &item)
{
    auto end = m_inventory.end(),
        it = std::find(m_inventory.begin(),
                       end, &item);

    if (it != end)
    {
        *it = *(end-1);
        m_inventory.pop_back();
    }
}
