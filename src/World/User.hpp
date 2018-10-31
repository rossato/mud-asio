#ifndef USER_HPP
#define USER_HPP

#include <memory>
#include <string>
#include <vector>

#include "Server/ConnectionBase.hpp"
#include "Noun.hpp"

namespace Mud
{
namespace Server
{
class ConnectionBase;
class Server;
}

namespace World
{
class Item;
class Location;
class World;

class User : public Noun
{
public:
    User(Mud::World::World &world, const std::string &name, Location &startingPlace);

    // Observer pointers are out there
    User(const User &)            = delete;
    User &operator=(const User &) = delete;

    void RegisterConnection(Server::ConnectionBase &connection);
    void DeregisterConnection(Server::ConnectionBase &connection);

    template <class T>
    void Broadcast(const T &message)
    {
        for (auto connection : m_connections)
        {
            connection->Write(message);
        }
    }

    const bool Verify(const std::string &) const { return true; }
    const bool IsPrivileged() const { return true; }

    Location &GetLocation() { return *m_here; }
    void SetLocation(Location *location) { m_here = location; }

    Mud::World::World &World() { return m_world; }
    
    const std::vector<Noun *> &GetInventory() const { return m_inventory; }
    void AddToInventory(Noun &item) { m_inventory.emplace_back(&item); }
    void RemoveFromInventory(Noun &);
    
private:
    Mud::World::World &m_world;
    Location *m_here;
    std::vector<Server::ConnectionBase *> m_connections;
    std::vector<Noun *> m_inventory;
};

}
}

#endif
