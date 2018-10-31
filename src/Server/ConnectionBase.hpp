#ifndef CONNECTION_BASE_HPP
#define CONNECTION_BASE_HPP

#include <functional>
#include <iostream>
#include <boost/asio.hpp>

namespace Mud
{
namespace Server
{

// This is a base class for a tcp-socket connection.
// This class implements writing and closing,
//  a superclass will implement reading and application logic.

// The superclass MUST respect m_socket.shutdown(tcp::socket::shutdown_receive)
//  and finish with a call to DoneReading() so that this class can destruct after
//  finishing pending writes.

class ConnectionBase {
public:
    typedef boost::asio::ip::tcp::socket SocketType;

    // Async callbacks are out there, don't copy or move.
    ConnectionBase(const ConnectionBase &)            = delete;
    ConnectionBase &operator=(const ConnectionBase &) = delete;

    template <class T>
    void Write(const T &message)
    {
        *m_outputStream << message;
        WriteToSocket();
    }

    template <class T>
    std::ostream &operator<<(const T &message)
    {
        Write(message);
        m_moreToWrite = true;
        return *m_outputStream;
    }

    std::ostream &ostream()
    {
        WriteToSocket();
        m_moreToWrite = true;
        return *m_outputStream;
    }

    // When the connection terminates, it will signal for its destruction.
    template <class Handler>
    void SetCloseHandler(Handler &&handler)
    {
        m_onClose = std::forward<Handler>(handler);
    }

    void Close(const std::string &reason);

    ///////////////////////////////////////////////////
    // For testing only, to circumvent the socket logic
    ConnectionBase(boost::asio::io_service &io_service, std::ostream &testOutput)
        : m_socket(io_service),
          m_outputStream1(&m_outputBuffer1),
          m_outputStream2(&m_outputBuffer2),
          m_outputStream(&testOutput),
          m_reading(true),
          m_writing(true), // This causes WriteToSocket to be a no-op
          m_moreToWrite(false)
    {}

protected:
    ConnectionBase(SocketType &&socket)
        : m_socket(std::move(socket)),
          m_outputStream1(&m_outputBuffer1),
          m_outputStream2(&m_outputBuffer2),
          m_outputBuffer(&m_outputBuffer1),
          m_bufferBeingWritten(&m_outputBuffer2),
          m_outputStream(&m_outputStream1),
          m_streamBeingWritten(&m_outputStream2),
          m_reading(true), m_writing(false), m_moreToWrite(false)
    {}
    ~ConnectionBase() = default;
    
    void DoneReading() { m_reading = false; WriteToSocket(); }

    SocketType m_socket;

private:
    void WriteToSocket();

    // Double-buffered writes
    boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
    std::ostream m_outputStream1, m_outputStream2;
    boost::asio::streambuf *m_outputBuffer, *m_bufferBeingWritten;
    std::ostream *m_outputStream, *m_streamBeingWritten;

    bool m_reading, m_writing, m_moreToWrite;
    std::function<void()> m_onClose;
};

}
}

#endif
