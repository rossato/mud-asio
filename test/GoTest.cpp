#include <gtest/gtest.h>
#include "Logic/BasicActions.hpp"
#include "HasWorld.hpp"
#include "HeadlessUser.hpp"

namespace Mud
{
namespace Test
{

class GoTest : public ::testing::Test, public WorldTest
{
protected:
    void ThenUserShouldBeInTheOtherPlace()
    {
        EXPECT_NE(ken.connection.output.str().find("Other Place"), std::string::npos);
    }
};

using namespace Mud::Logic;
    
TEST_F(GoTest, DumpGoAction)
{
    GoAction::Act(ken, ken.connection.output, Mud::World::E, 0);
    std::cout << ken.connection.output.str() << std::endl;

    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, GoDirActionWorks)
{
    GoDirAction<Mud::World::E>::Act(ken, ken.connection.output, 0, 0);
    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, GoBadDirectionFails)
{
    Mud::World::Location *before = &ken.user.GetLocation();
    GoAction::Act(ken, ken.connection.output, Mud::World::N, 0);
    EXPECT_EQ(before, &ken.user.GetLocation());
}

TEST_F(GoTest, DirectionTokenMatches)
{
    std::string line("go east");
    ken.tokenizer.Str(line);
    ken.tokenizer.GetString();
    EXPECT_EQ(
        Mud::World::E,
        DirectionMatcher::Match(ken, ken.tokenizer)
        );
}

TEST_F(GoTest, BadDirectionDoesntMatch)
{
    std::string line("go about");
    ken.tokenizer.Str(line);
    ken.tokenizer.GetString();
    EXPECT_EQ(
        Mud::World::NODIR,
        DirectionMatcher::Match(ken, ken.tokenizer)
        );
}

TEST_F(GoTest, GoCommandParses)
{
    ken.RunCommand("go east");
    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, GoCommandParsesBlankLine)
{
    ken.RunCommand("go");
    EXPECT_NE(std::string::npos, ken.connection.output.str().find("Go where?"));
}

TEST_F(GoTest, ShortcutCommandsParse)
{
    ken.RunCommand("e");
    ThenUserShouldBeInTheOtherPlace();
}

TEST_F(GoTest, UserObservesEntrance)
{
    HeadlessUser paul(world.GetUser("paul"));
    ken.RunCommand("e");
    EXPECT_NE(std::string::npos, paul.connection.output.str().find("Ken arrives in the area."));
}

TEST_F(GoTest, UserObservesExit)
{
    ken.RunCommand("e");
    HeadlessUser paul(world.GetUser("paul"));
    ken.RunCommand("w");
    EXPECT_NE(std::string::npos, paul.connection.output.str().find("Ken leaves to the west."));
}

}
}
