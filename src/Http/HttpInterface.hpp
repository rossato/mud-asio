#ifndef HTTP_INTERFACE_HPP
#define HTTP_INTERFACE_HPP

#include <map>
#include <string>

#include "Server/ConnectionBase.hpp"

namespace Mud
{
namespace Server
{
class Server;
}

namespace Http
{
    
class HttpInterface : public Server::ConnectionBase
{
public:
    template <class... Args>
    HttpInterface(Server::Server &server, Args &&... args)
        : Server::ConnectionBase(std::forward<Args>(args)...),
          m_server(server)
    {}

    void HandleLine(const std::string &line);
    void HandleClose() {}
    
private:
    void ExecuteRequest();

    Server::Server &m_server;
    std::map<std::string, std::string> m_requestHeaders;
    std::string m_method, m_url, m_version;
};

}
}
   
#endif
