#ifndef HTTP_CONNECTION_FACTORY_HPP
#define HTTP_CONNECTION_FACTORY_HPP

#include "Server/LineOrientedConnection.hpp"
#include "HttpInterface.hpp"

namespace Mud
{
namespace Server
{
    class Server;
}

namespace Http
{

class HttpConnectionFactory
{
public:
    typedef Server::LineOrientedConnection<HttpInterface> ConnectionType;
    static const std::string ConnectionTypeDescription; // "http"

    HttpConnectionFactory(Server::Server &server)
        : m_server(server) {}
    
    template <class ConnectionPoolType>
    void CreateConnection(ConnectionPoolType &pool, ConnectionType::SocketType &&socket)
    {
        pool.EmplaceConnection(std::move(socket), m_server);
    }

private:
    Server::Server &m_server;
};

}
}

#endif
