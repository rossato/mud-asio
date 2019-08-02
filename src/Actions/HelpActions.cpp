#include "HelpActions.hpp"

#include <iomanip>
#include "Grammar/Grammar.hpp"
#include "Interface/MudInterface.hpp"
#include "Server/Ansi.hpp"
#include "World/World.hpp"

using namespace Mud;
using namespace Mud::Actions;
    
const std::string      HelpAction::Description("General help information");
const std::string HelpAboutAction::Description("Information about the game");
const std::string  HelpVerbAction::Description("Get usage for a particular verb");

void HelpAction::Act(InterfaceType &interface, int, int)
{
    interface <<
        "For information about the game, try \"help about\"." NEWLINE NEWLINE
              <<
        "For help with a specific command, try \"help <command>\"." NEWLINE
        "This is the list of known commands:" NEWLINE;

    int count = 0;
    for (const auto &grammar : interface.Grammar().GrammarIndex())
    {
        if (++count > 7)
        {
            interface.Write(NEWLINE);
            count -= 7;
        }
        interface << std::left << std::setw(10) << grammar.first;
    }
    interface.Write(NEWLINE);
}

void HelpAboutAction::Act(Interface::MudInterface &interface, int, int)
{
    interface.Write(
        "mud-asio: C++11 MUD Project using the Boost ASIO library," NEWLINE
        " Copyright 2018 Ken Rossato" NEWLINE);
}

void HelpVerbAction::Act(Interface::MudInterface &interface,
                         Grammar::VerbMatcher::ValueType verb, int)
{
    if (*verb == "about")
    {
        HelpAboutAction::Act(interface, 0, 0);
        return;
    }

    const auto &grammarIndex = interface.Grammar().GrammarIndex();
    const auto verbHelp = grammarIndex.find(*verb);

    if (verbHelp == grammarIndex.end())
    {
        interface << "Help error: \"" << *verb << "\" is not a known command." NEWLINE;
    }
    else
    {
        interface.Write(verbHelp->second.help);
    }
}
