#ifndef TEST_CONNECTION_HPP
#define TEST_CONNECTION_HPP

#include <sstream>
#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include "Interface/MudInterface.hpp"
#include "Server/ConnectionBase.hpp"

namespace Mud
{
namespace Grammar
{
class Grammar;
}
namespace Server
{
class Server;
}
namespace World
{
class World;
}

namespace Test
{

class TestConnectionBase
{
protected:
    boost::asio::io_service m_io_service;

public:
    std::ostringstream output;
};

class TestConnection : public TestConnectionBase, public Mud::Server::ConnectionBase
{
public:
    TestConnection()
        : Mud::Server::ConnectionBase(m_io_service, output) {}
};

class TestInterface : public TestConnectionBase, public Mud::Interface::MudInterface
{
public:
    TestInterface();

    TestInterface(const std::string &name)
        : TestInterface()
    {
        HandleLine(name);
        HandleLine("password");

        EXPECT_NE(output.str().find("> "), std::string::npos);
        output.str("");
    }

    ~TestInterface()
    {
        HandleClose();
    }
};

}
}

#endif
