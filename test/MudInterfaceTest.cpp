#include <gtest/gtest.h>
#include "Actions/BasicActions.hpp"
#include "Interface/MudInterface.hpp"
#include "World/World.hpp"
#include "World/User.hpp"
#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

class MudInterfaceTest : public ::testing::Test, public HasWorld
{
};

TEST_F(MudInterfaceTest, DumpLoginScreen)
{
    TestInterface paul;
    paul.HandleLine("paul");
    paul.HandleLine("p");

    std::cout << paul.output.str() << std::endl;

    EXPECT_NE(paul.output.str().find("> "), std::string::npos);
}

TEST_F(MudInterfaceTest, CommandsWorkAfterLogin)
{
    ken.HandleLine("help");
    EXPECT_NE(
        ken.output.str().find("For information about the game"),
        std::string::npos);  
}

TEST_F(MudInterfaceTest, UserObservesLogin)
{
    ken.HandleLine("e");

    TestInterface paul("paul");

    EXPECT_NE(ken.output.str().find("Paul arrives in the area."), std::string::npos);
}

TEST_F(MudInterfaceTest, UserObservesLogout)
{
    {
        TestInterface paul("paul");
        paul.HandleLine("w");
    }

    EXPECT_NE(ken.output.str().find("Paul leaves to somewhere."), std::string::npos);
}

TEST_F(MudInterfaceTest, SecondLogin)
{
    ken.HandleLine("e");

    TestInterface paul("paul");

    paul.output.str("");
    ken.output.str("");
    
    TestInterface ken2("ken");
    
    EXPECT_EQ(paul.output.str().find("Ken arrives in the area."), std::string::npos);
    EXPECT_NE(ken.output.str().find("New login detected for user Ken"), std::string::npos);
}

TEST_F(MudInterfaceTest, SecondLogout)
{
    ken.HandleLine("e");

    {
        TestInterface paul2("paul");

        {
            TestInterface paul("paul");
            paul.output.str("");
            paul2.output.str("");
            ken.output.str("");
        }

        EXPECT_EQ(ken.output.str().find("Paul leaves to somewhere."), std::string::npos);
        ken.output.str("");
    }

    EXPECT_NE(ken.output.str().find("Paul leaves to somewhere."), std::string::npos);
}

}
}
