#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include "ConnectionBase.hpp"

namespace Mud
{
namespace Server
{
    
template <class InterfaceType>
class LineOrientedConnection : public InterfaceType {
public:
    template <class... Args>
    LineOrientedConnection(typename InterfaceType::SocketType &&socket, Args &&... args)
        : InterfaceType(std::forward<Args>(args)..., std::move(socket)),
          m_inputStream(&m_inputBuffer)
    {
        ReadLine();
    }

private:
    void ReadLine();

    boost::asio::streambuf m_inputBuffer;
    std::istream m_inputStream;
    std::string m_line;
};

}
}

#include "LineOrientedConnection-inl.hpp"

#endif
