#ifndef MUD_CONNECTION_FACTORY_HPP
#define MUD_CONNECTION_FACTORY_HPP

#include "Interface/MudInterface.hpp"
#include "Server/LineOrientedConnection.hpp"

namespace Mud
{
namespace Grammar
{
    class Grammar;
}
namespace Server
{
    class Server;
}
namespace World
{
    class World;
}

namespace Program
{

class MudConnectionFactory
{
public:
    typedef Server::LineOrientedConnection<Interface::MudInterface> ConnectionType;
    static const std::string ConnectionTypeDescription; // "mud telnet"

    MudConnectionFactory(Grammar::Grammar &grammar, Server::Server &server, World::World &world)
        : m_grammar(grammar), m_server(server), m_world(world)
    {}
    
    template <class ConnectionPoolType>
    void CreateConnection(ConnectionPoolType &pool, ConnectionType::SocketType &&socket)
    {
        pool.EmplaceConnection(std::move(socket), m_grammar, m_server, m_world);
    }

private:
    Grammar::Grammar &m_grammar;
    Server::Server &m_server;
    World::World &m_world;
};

}
}

#endif
