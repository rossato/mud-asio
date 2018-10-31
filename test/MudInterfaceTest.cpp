#include <gtest/gtest.h>
#include "Logic/BasicActions.hpp"
#include "Logic/MudInterface.hpp"
#include "Server/CloseException.hpp"
#include "World/World.hpp"
#include "World/User.hpp"
#include "HasWorld.hpp"
#include "TestConnection.hpp"

namespace Mud
{
namespace Test
{

class MudInterfaceTest : public ::testing::Test, public WorldTest
{
protected:
    MudInterfaceTest() : ken(world, "ken") {}

    TestConnectionWithInterface ken;
};

TEST_F(MudInterfaceTest, DumpLoginScreen)
{
    TestConnectionWithInterface paul(world);
    paul.mud.HandleLine("paul");
    paul.mud.HandleLine("p");

    std::cout << paul.output.str() << std::endl;

    EXPECT_NE(paul.output.str().find("> "), std::string::npos);
}

TEST_F(MudInterfaceTest, CommandsWorkAfterLogin)
{
    ken.mud.HandleLine("help");
    EXPECT_NE(
        ken.output.str().find("For information about the game"),
        std::string::npos);  
}

TEST_F(MudInterfaceTest, QuitThrowsException)
{
    EXPECT_THROW(ken.mud.HandleLine("quit"), Mud::Server::CloseException);
}

TEST_F(MudInterfaceTest, UserObservesLogin)
{
    ken.mud.HandleLine("e");

    TestConnectionWithInterface paul(world, "paul");
    
    EXPECT_NE(ken.output.str().find("Paul arrives in the area."), std::string::npos);
}

TEST_F(MudInterfaceTest, UserObservesLogout)
{
    {
        TestConnectionWithInterface paul(world, "paul");
        paul.mud.HandleLine("w");
        EXPECT_THROW(paul.mud.HandleLine("quit"), Mud::Server::CloseException);
    }

    EXPECT_NE(ken.output.str().find("Paul leaves to somewhere."), std::string::npos);
}

TEST_F(MudInterfaceTest, SecondLogin)
{
    ken.mud.HandleLine("e");

    TestConnectionWithInterface paul(world, "paul");

    paul.output.str("");
    ken.output.str("");
    
    TestConnectionWithInterface ken2(world, "ken");
    
    EXPECT_EQ(paul.output.str().find("Ken arrives in the area."), std::string::npos);
    EXPECT_NE(ken.output.str().find("New login detected for user Ken"), std::string::npos);
}

TEST_F(MudInterfaceTest, SecondLogout)
{
    ken.mud.HandleLine("e");

    auto paul  = std::make_unique<TestConnectionWithInterface>(world, "paul");
    auto paul2 = std::make_unique<TestConnectionWithInterface>(world, "paul");

    paul->output.str("");
    paul2->output.str("");
    ken.output.str("");

    EXPECT_THROW(paul->mud.HandleLine("quit"), Mud::Server::CloseException);
    paul.reset();

    EXPECT_EQ(ken.output.str().find("Paul leaves to somewhere."), std::string::npos);

    EXPECT_THROW(paul2->mud.HandleLine("quit"), Mud::Server::CloseException);
    paul2.reset();

    EXPECT_NE(ken.output.str().find("Paul leaves to somewhere."), std::string::npos);
}

}
}
