#include <gtest/gtest.h>
#include "Logic/HelpActions.hpp"
#include "HasWorld.hpp"
#include "HeadlessUser.hpp"

namespace Mud
{
namespace Test
{

class HelpTest : public ::testing::Test, public WorldTest
{
};

using namespace Mud::Grammar;
using namespace Mud::Logic;

TEST_F(HelpTest, DumpHelpAction)
{
    HelpAction::Act(ken, ken.connection.output, 0, 0);
    std::cout << ken.connection.output.str() << std::endl;

    EXPECT_NE(ken.connection.output.str().find("For information about the game"),
              std::string::npos);
    EXPECT_NE(ken.connection.output.str().find("quit"), std::string::npos);
}

TEST_F(HelpTest, DumpHelpAboutAction)
{
    HelpAboutAction::Act(ken, ken.connection.output, 0, 0);
    std::cout << ken.connection.output.str() << std::endl;

    EXPECT_NE(ken.connection.output.str().find("C++17 MUD Teaching Exercise"),
              std::string::npos);
}

TEST_F(HelpTest, DumpHelpVerbAction)
{
    std::string help("help");
    HelpVerbAction::Act(ken, ken.connection.output, &help, 0);
    std::cout << ken.connection.output.str() << std::endl;

    EXPECT_NE(ken.connection.output.str().find("Get usage for a particular verb"), std::string::npos);
}

TEST_F(HelpTest, VerbTokenMatches)
{
    std::string line("help help");
    ken.tokenizer.Str(line);

    auto value = VerbMatcher::Match(ken, ken.tokenizer);

    ASSERT_TRUE(!!value);
    EXPECT_EQ(*value, std::string("help"));
}

TEST_F(HelpTest, HelpCommandParses)
{
    ken.RunCommand("help");
    EXPECT_NE(ken.connection.output.str().find("For information about the game"),
              std::string::npos);
}

TEST_F(HelpTest, HelpAboutCommandParses)
{
    ken.RunCommand("help about");
    EXPECT_NE(ken.connection.output.str().find("C++17 MUD Teaching Exercise"),
              std::string::npos);
}

TEST_F(HelpTest, HelpVerbCommandParses)
{
    ken.RunCommand("help help");
    EXPECT_NE(ken.connection.output.str().find("Get usage for a particular verb"), std::string::npos);
}

TEST_F(HelpTest, HelpVerbRejectsUnknownVerbs)
{
    ken.RunCommand("help foo");
    EXPECT_NE(ken.connection.output.str().find("\"foo\" is not a known command"), std::string::npos);
}

}
}
