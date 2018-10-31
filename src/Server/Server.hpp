#ifndef SERVER_HPP
#define SERVER_HPP

#include <numeric>
#include <boost/asio.hpp>
#include "Acceptor.hpp"

namespace Mud
{
namespace Server
{

class Server {
public:
    Server();

    template <class ConnectionFactoryType>
    void Accept(int port, ConnectionFactoryType &factory)
    {
        m_acceptors.emplace_back(
            std::make_unique<Acceptor<ConnectionFactoryType> >(m_io_service, port, factory)
            );
        std::cout << "Server created acceptor for \""
                  << ConnectionFactoryType::ConnectionTypeDescription
                  << "\" on port " << port << "." << std::endl;
    }

    // Blocks until interrupted by a signal or otherwise Shutdown().
    void Run();

    void CancelShutdown();
    void Shutdown(int seconds);

    std::pair<int, int> GetNumberOfConnections() const;
    
private:
    void DoShutdown();

    std::vector<std::unique_ptr<IAcceptor> > m_acceptors;

    boost::asio::io_service m_io_service;
    boost::asio::deadline_timer m_shutdownTimer;
    boost::asio::signal_set m_signal_set;
};
    
}
}

#endif
