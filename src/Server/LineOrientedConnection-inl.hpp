#include <iostream>

namespace Mud
{
namespace Server
{

template<class InterfaceType>
void LineOrientedConnection<InterfaceType>::ReadLine()
{
    async_read_until(this->m_socket, m_inputBuffer, '\n',
    [this](boost::system::error_code error, std::size_t)
    {
        if (error)
        {
            if (error != boost::asio::error::eof)
            {
                std::cout << "async_read_until returned with error: " << error << std::endl;
            }

            this->HandleClose();
            this->DoneReading();
        }
        else
        {
            std::getline(m_inputStream, m_line);
            this->HandleLine(m_line);

            ReadLine();
        }
    });
}

}
}
