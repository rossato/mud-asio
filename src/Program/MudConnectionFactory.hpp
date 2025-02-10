#ifndef MUD_CONNECTION_FACTORY_HPP
#define MUD_CONNECTION_FACTORY_HPP

#include "Interface/MudInterface.hpp"
#include "Server/LineOrientedConnection.hpp"

namespace Mud
{
namespace Program
{

class MudProgram;

class MudConnectionFactory
{
public:
    typedef Server::LineOrientedConnection<Interface::MudInterface> ConnectionType;
    static const std::string ConnectionTypeDescription; // "mud telnet"

    MudConnectionFactory(MudProgram &program)
        : m_program(program)
    {}
    
    template <class ConnectionPoolType>
    void CreateConnection(ConnectionPoolType &pool, ConnectionType::SocketType &&socket)
    {
        pool.EmplaceConnection(std::move(socket), m_program);
    }

private:
    MudProgram &m_program;
};

}
}

#endif
