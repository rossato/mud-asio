#include <gtest/gtest.h>
#include "Logic/BasicActions.hpp"
#include "HasWorld.hpp"
#include "HeadlessUser.hpp"

namespace Mud
{
namespace Test
{

class SayTest : public ::testing::Test, public WorldTest
{
};

using namespace Mud::Grammar;
using namespace Mud::Logic;
    
TEST_F(SayTest, DumpSayAction)
{
    RestOfLineMatcher::ValueType value("hello");
    SayAction::Act(ken, ken.connection.output, value, 0);
    std::cout << ken.connection.output.str() << std::endl;

    EXPECT_NE(ken.connection.output.str().find("You say"), std::string::npos);
    EXPECT_NE(ken.connection.output.str().find("hello"), std::string::npos);
}

TEST_F(SayTest, DumpSayActionObserved)
{
    HeadlessUser paul(world.GetUser("paul"));
    paul.RunCommand("w");

    ken.connection.output.str("");
    RestOfLineMatcher::ValueType value("hello");
    SayAction::Act(paul, paul.connection.output, value, 0);

    std::cout << ken.connection.output.str() << std::endl;

    EXPECT_NE(ken.connection.output.str().find("Paul says"), std::string::npos);
    EXPECT_NE(ken.connection.output.str().find("hello"), std::string::npos);
}

TEST_F(SayTest, Parse)
{
    ken.RunCommand("say Hello, everyone!");
    EXPECT_NE(ken.connection.output.str().find("You say"), std::string::npos);
    EXPECT_NE(ken.connection.output.str().find("Hello, everyone!"), std::string::npos);
}

TEST_F(SayTest, ParseBlankLine)
{
    ken.RunCommand("say");
    EXPECT_NE(ken.connection.output.str().find("Say what?"), std::string::npos);
}

}
}
