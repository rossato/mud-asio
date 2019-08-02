#include <gtest/gtest.h>
#include "Actions/BasicActions.hpp"
#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

class LookTest : public ::testing::Test, public HasWorld
{
};

using namespace Mud::Actions;

TEST_F(LookTest, DumpLookAction)
{
    LookAction::Act(ken, 0, 0);
    std::cout << ken.output.str() << std::endl;

    EXPECT_NE(ken.output.str().find("Place"), std::string::npos);
}

TEST_F(LookTest, Parse)
{
    ken.HandleLine("look");
    EXPECT_NE(ken.output.str().find("Place"), std::string::npos);
}

TEST_F(LookTest, ObserveOtherUser)
{
    TestInterface paul("paul");
    paul.HandleLine("w");

    ken.output.str("");
    LookAction::Act(ken, 0, 0);

    EXPECT_NE(ken.output.str().find("Paul is here."), std::string::npos);
}

}
}
