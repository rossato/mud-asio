#ifndef TEST_CONNECTION_HPP
#define TEST_CONNECTION_HPP

#include <sstream>
#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include "Server/ConnectionBase.hpp"
#include "Logic/MudInterface.hpp"

namespace Mud
{
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

class TestConnectionWithInterface : public TestConnection
{
public:
    TestConnectionWithInterface(World::World &world)
        : mud(*this, world) {}

    TestConnectionWithInterface(World::World &world, const std::string &name)
        : TestConnectionWithInterface(world)
    {
        mud.HandleLine(name);
        mud.HandleLine("p");

        EXPECT_NE(output.str().find("> "), std::string::npos);
        output.str("");
    }
    
    Mud::Logic::MudInterface mud;
};

}
}

#endif
