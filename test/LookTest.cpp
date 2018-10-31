#include <gtest/gtest.h>
#include "Logic/BasicActions.hpp"
#include "HasWorld.hpp"
#include "HeadlessUser.hpp"

namespace Mud
{
namespace Test
{

class LookTest : public ::testing::Test, public WorldTest
{
};

using namespace Mud::Logic;
    
TEST_F(LookTest, DumpLookAction)
{
    LookAction::Act(ken, ken.connection.output, 0, 0);
    std::cout << ken.connection.output.str() << std::endl;

    EXPECT_NE(ken.connection.output.str().find("Place"), std::string::npos);
}

TEST_F(LookTest, Parse)
{
    ken.RunCommand("look");
    EXPECT_NE(ken.connection.output.str().find("Place"), std::string::npos);
}

TEST_F(LookTest, ObserveOtherUser)
{
    HeadlessUser paul(world.GetUser("paul"));
    paul.RunCommand("w");

    ken.connection.output.str("");
    LookAction::Act(ken, ken.connection.output, 0, 0);

    EXPECT_NE(ken.connection.output.str().find("Paul is here."), std::string::npos);
}

}
}
