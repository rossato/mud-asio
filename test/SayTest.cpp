#include <gtest/gtest.h>
#include "Actions/BasicActions.hpp"
#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

class SayTest : public ::testing::Test, public HasWorld
{
};

using namespace Mud::Actions;
using namespace Mud::Grammar;

TEST_F(SayTest, DumpSayAction)
{
    RestOfLineMatcher::ValueType value("hello");
    SayAction::Act(ken, value, 0);
    std::cout << ken.output.str() << std::endl;

    EXPECT_NE(ken.output.str().find("You say"), std::string::npos);
    EXPECT_NE(ken.output.str().find("hello"), std::string::npos);
}

TEST_F(SayTest, DumpSayActionObserved)
{
    TestInterface paul("paul");
    paul.HandleLine("w");

    ken.output.str("");
    RestOfLineMatcher::ValueType value("hello");
    SayAction::Act(paul, value, 0);

    std::cout << ken.output.str() << std::endl;

    EXPECT_NE(ken.output.str().find("Paul says"), std::string::npos);
    EXPECT_NE(ken.output.str().find("hello"), std::string::npos);
}

TEST_F(SayTest, Parse)
{
    ken.HandleLine("say Hello, everyone!");
    EXPECT_NE(ken.output.str().find("You say"), std::string::npos);
    EXPECT_NE(ken.output.str().find("Hello, everyone!"), std::string::npos);
}

TEST_F(SayTest, ParseBlankLine)
{
    ken.HandleLine("say");
    EXPECT_NE(ken.output.str().find("Say what?"), std::string::npos);
}

}
}
