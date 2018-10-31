#include <iostream>
#include "CloseException.hpp"

namespace Mud
{
namespace Server
{

template<class InterfaceType>
void LineOrientedConnection<InterfaceType>::ReadLine()
{
    async_read_until(m_socket, m_inputBuffer, '\n',
    [this](boost::system::error_code error, std::size_t)
    {
        if (error)
        {
            std::cout << "async_read_until returned with error." << std::endl;
        }
        else
        {
            try
            {
                std::getline(m_inputStream, m_line);
                m_interface.HandleLine(m_line);

                ReadLine();
                return;
            }
            catch (const CloseException &)
            {
                std::cout << "Gracefully closing connection due to program logic." << std::endl;
            }
        }
        DoneReading();
    });
}

}
}
