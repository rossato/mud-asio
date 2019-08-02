#include <gtest/gtest.h>
#include "Actions/BasicActions.hpp"
#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

class GoTest : public ::testing::Test, public HasWorld
{
protected:
    void ThenUserShouldBeInTheOtherPlace()
    {
        EXPECT_NE(ken.output.str().find("Other Place"), std::string::npos);
    }
};

using namespace Mud::Actions;

TEST_F(GoTest, DumpGoAction)
{
    GoAction::Act(ken, Mud::World::E, 0);
    std::cout << ken.output.str() << std::endl;

    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, GoDirActionWorks)
{
    GoDirAction<Mud::World::E>::Act(ken, 0, 0);
    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, GoBadDirectionFails)
{
    Mud::World::Location *before = ken.User().GetLocation();
    GoAction::Act(ken, Mud::World::N, 0);
    EXPECT_EQ(before, ken.User().GetLocation());
}

TEST_F(GoTest, DirectionTokenMatches)
{
    std::string line("go east");
    ken.Str(line);
    ken.GetString();
    EXPECT_EQ(
        Mud::World::E,
        DirectionMatcher::Match(ken)
        );
}

TEST_F(GoTest, BadDirectionDoesntMatch)
{
    std::string line("go about");
    ken.Str(line);
    ken.GetString();
    EXPECT_EQ(
        Mud::World::NODIR,
        DirectionMatcher::Match(ken)
        );
}

TEST_F(GoTest, GoCommandParses)
{
    ken.HandleLine("go east");
    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, GoCommandParsesBlankLine)
{
    ken.HandleLine("go");
    EXPECT_NE(std::string::npos, ken.output.str().find("Go where?"));
}

TEST_F(GoTest, ShortcutCommandsParse)
{
    ken.HandleLine("e");
    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, UserObservesEntrance)
{
    TestInterface paul("paul");
    ken.HandleLine("e");
    EXPECT_NE(std::string::npos, paul.output.str().find("Ken arrives in the area."));
}

TEST_F(GoTest, UserObservesExit)
{
    ken.HandleLine("e");
    TestInterface paul("paul");
    ken.HandleLine("w");
    EXPECT_NE(std::string::npos, paul.output.str().find("Ken leaves to the west."));
}

}
}
