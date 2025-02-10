#include "HelpActions.hpp"

#include <iomanip>
#include "Interface/MudInterface.hpp"
#include "Parser/Parser.hpp"
#include "Server/Ansi.hpp"
#include "World/World.hpp"

using namespace Mud;
using namespace Mud::Actions;
    
const std::string      HelpAction::Description("General help information");
const std::string HelpAboutAction::Description("Information about the game");
const std::string  HelpVerbAction::Description("Get usage for a particular verb");

void HelpAction::Act(Interface::MudInterface &interface)
{
    interface <<
        "For information about the game, try \"help about\"." NEWLINE NEWLINE
              <<
        "For help with a specific command, try \"help <command>\"." NEWLINE
        "This is the list of known commands:" NEWLINE;

    int count = 0;
    for (const auto &grammar : interface.Parser().GrammarIndex())
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

void HelpAboutAction::Act(Interface::MudInterface &interface)
{
    interface.Write(
        "mud-asio: C++11 MUD Project using the Boost ASIO library," NEWLINE
        " Copyright 2018 Ken Rossato" NEWLINE);
}

void HelpVerbAction::Act(Interface::MudInterface &interface,
                         Parser::VerbMatcher::ValueType verb)
{
    if (*verb == "about")
    {
        HelpAboutAction::Act(interface);
        return;
    }

    const auto &grammarIndex = interface.Parser().GrammarIndex();
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
