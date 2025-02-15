#ifndef CONNECTION_POOL_HPP
#define CONNECTION_POOL_HPP

#include <iostream>
#include <list>

namespace Mud
{
namespace Server
{

template <class ConnectionFactoryType>
class ConnectionPool
{
    typedef typename ConnectionFactoryType::ConnectionType ConnectionType;

public:
    ConnectionPool(ConnectionFactoryType &factory)
        : m_factory(factory), m_peakConcurrentConnections(0) {}

    void OpenNewConnection(boost::asio::ip::tcp::socket &&socket)
    {
        auto newConnection = m_closedConnections.begin();
        if (newConnection == m_closedConnections.end())
        {
            m_factory.CreateConnection(*this, std::move(socket));
            newConnection = m_connections.begin();
        }
        else
        {
            // This is a questionable optimization
            //  because if done incorrectly reusing objects can be a security issue.
            // Nonetheless this reduces allocations and was fun to implement.

            *newConnection = std::move(socket);
            m_connections.splice(m_connections.begin(), m_closedConnections, newConnection);
        }

        std::cout << "ConnectionPool(" << ConnectionFactoryType::ConnectionTypeDescription
                  << ") is opening connection #" << newConnection->ConnectionNumber() << ", total is now "
                  << m_connections.size() << "." << std::endl;

        if (m_connections.size() > m_peakConcurrentConnections)
        {
            m_peakConcurrentConnections = m_connections.size();
            std::cout << "ConnectionPool(" << ConnectionFactoryType::ConnectionTypeDescription
                      << ") has new peak connection record: " << m_peakConcurrentConnections
                      << "." << std::endl;
        }

        newConnection->SetCloseHandler(
        [this, newConnection]
        {
            m_closedConnections.splice(m_closedConnections.end(), m_connections, newConnection);
            std::cout << "ConnectionPool(" << ConnectionFactoryType::ConnectionTypeDescription
                      << ") has closed connection #" << newConnection->ConnectionNumber()
                      << ", " << m_connections.size() << " remaining." << std::endl;
        });
    }

    // Implement the factory pattern without pointers
    template <class... Args>
    void EmplaceConnection(Args &&... args)
    {
        m_connections.emplace_front(std::forward<Args>(args)...);
    }

    void CloseAll()
    {
        for (auto &connection : m_connections)
        {
            connection.Close("Server shutdown");
        }
    }

    std::pair<unsigned int, unsigned int> Size() const
    {
        return std::make_pair(m_connections.size(), m_peakConcurrentConnections);
    }
    
private:
    std::list<ConnectionType> m_connections;
    std::list<ConnectionType> m_closedConnections;
    ConnectionFactoryType &m_factory;
    unsigned int m_peakConcurrentConnections;
};
    
}
}

#endif
