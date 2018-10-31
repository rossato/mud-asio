#ifndef CONNECTION_POOL_HPP
#define CONNECTION_POOL_HPP

#include <iostream>
#include <list>
#include <boost/pool/pool_alloc.hpp>

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
        m_factory.CreateConnection(*this, std::move(socket));

        std::cout << "ConnectionPool(" << ConnectionFactoryType::ConnectionTypeDescription
                  << ") is opening connection, total is now "
                  << m_connections.size() << "." << std::endl;

        if (m_connections.size() > m_peakConcurrentConnections)
        {
            m_peakConcurrentConnections = m_connections.size();
            std::cout << "ConnectionPool(" << ConnectionFactoryType::ConnectionTypeDescription
                      << ") has new peak connection record: " << m_peakConcurrentConnections
                      << "." << std::endl;
        }

        auto newConnection = m_connections.begin();
        newConnection->SetCloseHandler(
        [this, newConnection]
        {
            m_connections.erase(newConnection);
            std::cout << "ConnectionPool(" << ConnectionFactoryType::ConnectionTypeDescription
                      << ") has removed connection, " << m_connections.size()
                      << " remaining." << std::endl;
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

    std::pair<int, int> Size() const
    {
        return std::make_pair(m_connections.size(), m_peakConcurrentConnections);
    }
    
private:
    std::list<ConnectionType, boost::fast_pool_allocator<ConnectionType> > m_connections;
    ConnectionFactoryType &m_factory;
    int m_peakConcurrentConnections;
};
    
}
}

#endif
