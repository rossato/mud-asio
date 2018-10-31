#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include "ConnectionBase.hpp"

namespace Mud
{
namespace Server
{
    
template <class InterfaceType>
class LineOrientedConnection : public ConnectionBase {
public:
    template <class... Args>
    LineOrientedConnection(SocketType &&socket, Args &&... args)
        : ConnectionBase(std::move(socket)),
          m_inputStream(&m_inputBuffer),
          m_interface(*this, std::forward<Args>(args)...)
    {
        ReadLine();
    }

private:
    void ReadLine();

    boost::asio::streambuf m_inputBuffer;
    std::istream m_inputStream;
    std::string m_line;

    InterfaceType m_interface;
};

}
}

#include "LineOrientedConnection-inl.hpp"

#endif
