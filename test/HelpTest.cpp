#include <gtest/gtest.h>
#include "Actions/HelpActions.hpp"
#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

class HelpTest : public ::testing::Test, public HasWorld
{
};

using namespace Mud::Grammar;
using namespace Mud::Actions;

TEST_F(HelpTest, DumpHelpAction)
{
    HelpAction::Act(ken, 0, 0);
    std::cout << ken.output.str() << std::endl;

    EXPECT_NE(ken.output.str().find("For information about the game"),
              std::string::npos);
    EXPECT_NE(ken.output.str().find("quit"), std::string::npos);
}

TEST_F(HelpTest, DumpHelpAboutAction)
{
    HelpAboutAction::Act(ken, 0, 0);
    std::cout << ken.output.str() << std::endl;

    EXPECT_NE(ken.output.str().find("C++11 MUD Project"),
              std::string::npos);
}

TEST_F(HelpTest, DumpHelpVerbAction)
{
    std::string help("help");
    HelpVerbAction::Act(ken, &help, 0);
    std::cout << ken.output.str() << std::endl;

    EXPECT_NE(ken.output.str().find("Get usage for a particular verb"), std::string::npos);
}

TEST_F(HelpTest, VerbTokenMatches)
{
    std::string line("help help");
    ken.Str(line);

    auto value = VerbMatcher::Match(ken);

    ASSERT_TRUE(!!value);
    EXPECT_EQ(*value, std::string("help"));
}

TEST_F(HelpTest, HelpCommandParses)
{
    ken.HandleLine("help");
    EXPECT_NE(ken.output.str().find("For information about the game"),
              std::string::npos);
}

TEST_F(HelpTest, HelpAboutCommandParses)
{
    ken.HandleLine("help about");
    EXPECT_NE(ken.output.str().find("C++11 MUD Project"),
              std::string::npos);
}

TEST_F(HelpTest, HelpVerbCommandParses)
{
    ken.HandleLine("help help");
    EXPECT_NE(ken.output.str().find("Get usage for a particular verb"), std::string::npos);
}

TEST_F(HelpTest, HelpVerbRejectsUnknownVerbs)
{
    ken.HandleLine("help foo");
    EXPECT_NE(ken.output.str().find("\"foo\" is not a known command"), std::string::npos);
}

}
}
