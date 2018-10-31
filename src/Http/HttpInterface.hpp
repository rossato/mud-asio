#ifndef HTTP_INTERFACE_HPP
#define HTTP_INTERFACE_HPP

#include <map>
#include <string>

namespace Mud
{
namespace Server
{
    class ConnectionBase;
    class Server;
}

namespace Http
{
    
class HttpInterface
{
public:
    HttpInterface(Server::ConnectionBase &connection, Server::Server &server)
        : m_connection(connection), m_server(server)
    {}

    void HandleLine(const std::string &line);

private:
    [[ noreturn ]] void ExecuteRequest();

    Server::ConnectionBase &m_connection;
    Server::Server &m_server;
    std::map<std::string, std::string> m_requestHeaders;
    std::string m_method, m_url, m_version;
};

}
}
   
#endif
