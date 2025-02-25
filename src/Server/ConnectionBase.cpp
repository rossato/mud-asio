#include <iostream>

#include "ConnectionBase.hpp"
#include "Ansi.hpp"

using namespace Mud::Server;

void ConnectionBase::Close(const std::string &reason)
{
    std::cout << "Connection #" << m_connectionNumber
              << " is closing for reason: " << reason << "." << std::endl;

    m_socket.shutdown(SocketType::shutdown_receive);
}

ConnectionBase &ConnectionBase::operator=(SocketType &&socket)
{
    // precondition: connection is closed!

    m_socket = std::move(socket);

    m_outputBuffer1.consume(m_outputBuffer1.size());
    m_outputBuffer2.consume(m_outputBuffer2.size());

    m_reading     = true;
    m_writing     = false;
    m_moreToWrite = false;

    return *this;
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

        if (!m_reading)
        {
            // This was previously a potential issue when m_onClose deleted "this".
            //  Workaround would be:
            //  boost::asio::defer(m_socket.get_executor(), std::move(m_onClose));

            m_socket.close();
            m_onClose();
        }
    });
}

unsigned int ConnectionBase::NumConnections(0);
