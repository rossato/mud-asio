#include "ConnectionBase.hpp"
#include "Ansi.hpp"

using namespace Mud::Server;

void ConnectionBase::Close(const std::string &reason)
{
    *m_outputStream << NEWLINE "Server is closing connection for reason: "
                    << reason << "." NEWLINE;
    WriteToSocket();

    m_socket.shutdown(SocketType::shutdown_receive);
}

void ConnectionBase::WriteToSocket()
{
    if (m_writing)
    {
        m_moreToWrite = true;
        return;
    }
    m_writing = true;

    std::swap(m_bufferBeingWritten, m_outputBuffer);
    std::swap(m_streamBeingWritten, m_outputStream);

    async_write(m_socket, *m_bufferBeingWritten,
    [this](boost::system::error_code error, std::size_t)
    {
        m_writing = false;

        if (error)
        {
            std::cout << "async_write returned with error." << std::endl;
            Close("Socket error");
        }
        else if (m_moreToWrite)
        {
            WriteToSocket();
            m_moreToWrite = false;
            return;
        }

        if (!m_reading) m_onClose();
    });
}
