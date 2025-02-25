#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include "ConnectionPool.hpp"
#include "UnrecoverableError.hpp"

namespace Mud
{
namespace Server
{

class IAcceptor
{
public:
    virtual ~IAcceptor() = default;
    virtual void Close() = 0;
    virtual std::pair<int, int> GetNumberOfConnections() const = 0;
};

template <class ConnectionFactoryType>
class Acceptor : public IAcceptor
{
public:
    Acceptor(boost::asio::io_service &io_service, int port, ConnectionFactoryType &factory)
        : m_connectionPool(factory),
          m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port)),
          m_nextSocket(io_service),
          m_port(port)
    {
        Accept();
    }

    // Async callbacks capturing "this", don't copy or move.
    Acceptor(const Acceptor &)            = delete;
    Acceptor &operator=(const Acceptor &) = delete;
    Acceptor(Acceptor &&)                 = delete;
    Acceptor &operator=(Acceptor &&)      = delete;

    virtual void Close() override
    {
        m_acceptor.close();
        m_connectionPool.CloseAll();
    }

    virtual std::pair<int, int> GetNumberOfConnections() const override
    {
        return m_connectionPool.Size();
    }
    
private:
    void Accept()
    {
        m_acceptor.async_accept(m_nextSocket,
        [this](boost::system::error_code error)
        {
            if (!error)
            {
                // std::moving into a new socket is fine,
                //  but moving into a previously *closed* socket is problematic.
                // The temporary here gives us an object to discard closed socket state.
                // Debugging this error was fun because it would consistently
                //  show up on the third connection.

                m_connectionPool.OpenNewConnection(
                    boost::asio::ip::tcp::socket(std::move(m_nextSocket))
                    );

                Accept();
            }
            else if (error != boost::asio::error::operation_aborted)
            {
                std::ostringstream out;
                out << "async_accept on port " << m_port
                    << " returned with error " << error
                    << std::endl;

                std::cout << out.rdbuf();
                throw UnrecoverableError(out.str());
            }
        });
    }

    ConnectionPool<ConnectionFactoryType> m_connectionPool;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_nextSocket;
    const int m_port;
};
 
}
}

#endif
