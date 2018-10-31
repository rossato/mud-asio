#ifndef MUD_CONNECTION_FACTORY_HPP
#define MUD_CONNECTION_FACTORY_HPP

#include "Server/LineOrientedConnection.hpp"
#include "MudInterface.hpp"

namespace Mud
{
namespace World
{
    class World;
}

namespace Logic
{

class MudConnectionFactory
{
public:
    typedef Server::LineOrientedConnection<MudInterface> ConnectionType;
    static const std::string ConnectionTypeDescription; // "mud telnet"

    MudConnectionFactory(World::World &world)
        : m_world(world) {}
    
    template <class ConnectionPoolType>
    void CreateConnection(ConnectionPoolType &pool, ConnectionType::SocketType &&socket)
    {
        pool.EmplaceConnection(std::move(socket), m_world);
    }

private:
    World::World &m_world;
};

}
}

#endif
