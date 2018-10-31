#include "HelpActions.hpp"

#include <iomanip>
#include "Grammar/Grammar.hpp"
#include "Server/Ansi.hpp"
#include "World/User.hpp"
#include "World/World.hpp"

using namespace Mud;
using namespace Mud::Logic;
    
const std::string      HelpAction::Description("General help information");
const std::string HelpAboutAction::Description("Information about the game");
const std::string  HelpVerbAction::Description("Get usage for a particular verb");

void HelpAction::Act(World::User &user, std::ostream &response, int, int)
{
    response << "For information about the game, try \"help about\"." NEWLINE NEWLINE;
    response <<
        "For help with a specific command, try \"help <command>\"." NEWLINE
        "This is the list of known commands:" NEWLINE;

    int count = 0;
    for (const auto &grammar : user.World().Grammar().GrammarIndex())
    {
        if (++count > 7)
        {
            response << NEWLINE;
            count -= 7;
        }
        response << std::left << std::setw(10) << grammar.first;
    }
    response << NEWLINE;
}

void HelpAboutAction::Act(World::User &, std::ostream &response, int, int)
{
    response <<
        "mud-asio: C++11 MUD Project using the Boost ASIO library," NEWLINE
        " Copyright 2018 Ken Rossato" NEWLINE;
}

void HelpVerbAction::Act(World::User &user, std::ostream &response,
                         Grammar::VerbMatcher::ValueType verb, int)
{
    if (*verb == "about")
    {
        HelpAboutAction::Act(user, response, 0, 0);
        return;
    }

    const auto &grammarIndex = user.World().Grammar().GrammarIndex();
    const auto verbHelp = grammarIndex.find(*verb);

    if (verbHelp == grammarIndex.end())
    {
        response << "Help error: \"" << *verb << "\" is not a known command." NEWLINE;
    }
    else
    {
        response << verbHelp->second.help;
    }
}
