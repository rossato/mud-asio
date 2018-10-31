#include "Server.hpp"

using namespace Mud::Server;

Server::Server()
    : m_io_service(),
      m_shutdownTimer(m_io_service),
      m_signal_set(m_io_service, SIGINT, SIGTERM)
{
}

// Blocks until interrupted by a signal or otherwise Shutdown().
void Server::Run()
{
    m_signal_set.async_wait(
    [this](boost::system::error_code error, int signal)
    {
        if (error != boost::asio::error::operation_aborted)
        {
            std::cout << "Server received signal(" << signal
                      << ") requesting shutdown." << std::endl;
            DoShutdown();
        }
    });

    std::cout << "Server started and ready to accept connections." << std::endl;
    m_io_service.run();

    std::cout << "Server is now finished." << std::endl;
}

void Server::CancelShutdown()
{
    std::cout << "Aborting any scheduled shutdown." << std::endl;
    m_shutdownTimer.cancel();
}

void Server::Shutdown(int seconds)
{
    std::cout << "Server is scheduling shutdown for " << seconds << " seconds from now." << std::endl;
    m_shutdownTimer.expires_from_now(boost::posix_time::seconds(seconds));
    m_shutdownTimer.async_wait(
    [this](boost::system::error_code e)
    {
        if (e != boost::asio::error::operation_aborted)
            DoShutdown();
    });
}

std::pair<int, int> Server::GetNumberOfConnections() const
{
    return std::accumulate(m_acceptors.begin(), m_acceptors.end(), std::make_pair(0,0),
    [](std::pair<int, int> total, const std::unique_ptr<IAcceptor> &acceptor)
    {
        auto sizes = acceptor->GetNumberOfConnections();
        return std::make_pair(total.first + sizes.first, std::max(total.second, sizes.second));
    });
}

void Server::DoShutdown()
{
    std::cout << "Server is shutting down..." << std::endl;

    // Kill every async job in the io_service
    m_signal_set.cancel();
    m_shutdownTimer.cancel();
    for (auto &acceptor : m_acceptors)
    {
        acceptor->Close();
    }

    // io_service.run() will terminate after operations finish
}
