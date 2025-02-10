#include "BasicActions.hpp"

#include <iomanip>
#include "Interface/ClosingState.hpp"
#include "Interface/MudInterface.hpp"
#include "Server/Ansi.hpp"
#include "World/Location.hpp"
#include "World/User.hpp"

using namespace Mud::Actions;

const std::string  DirectionMatcher::Description("<direction>");
    
const std::string      LookAction::Description("Look around the current area");
const std::string      QuitAction::Description("Quit the MUD");
const std::string        GoAction::Description("Travel in a direction");
const std::string GoNowhereAction::Description("Prompt for a direction to go");
const std::string       SayAction::Description("Speak a line of text to those in the same area as you");

void QuitAction::Act(Interface::MudInterface &interface)
{
    interface.Write("Goodbye!" NEWLINE);
    interface.Close("user request");
    interface.ChangeState<Interface::ClosingState>();
}

void LookAction::Act(Interface::MudInterface &interface)
{
    auto &user = interface.User();

    interface << NEWLINE << *user.GetLocation();
    for (const auto otherUser : user.GetLocation()->GetUsers())
    {
        if (&*otherUser != &user)
        {
            // TODO combine the users present into a single grammatic sentence.
            interface << GREENTEXT << otherUser->Name() << " is here." PLAINTEXT NEWLINE;
        }
    }
    for (const auto item : user.GetLocation()->GetItems())
    {
        interface << "There is " << item->aName() << " here." NEWLINE;
    }
}

void GoAction::Act(Interface::MudInterface &interface, typename DirectMatcher::ValueType dir)
{
    if (dir == World::NODIR)
    {
        interface.Write("Go where?" NEWLINE);
        return;
    }

    auto &user = interface.User();
    if (World::Noun *location = user.GetLocation()->GetExit(dir))
    {
        user.GetLocation()->UserLeaving(user, dir);
        user.SetLocation(location);

        interface << "You leave to " << dir << "." NEWLINE;
        LookAction::Act(interface);

        location->UserArriving(user);
    }
    else
    {
        interface.Write("There's nothing in that direction." NEWLINE);
    }
}

void SayAction::Act(Interface::MudInterface &interface, const typename DirectMatcher::ValueType &line)
{
    if (line->empty())
    {
        interface.Write("Say what?" NEWLINE);
        return;
    }

    auto &user = interface.User();

    std::ostringstream out;
    out << MAGENTATEXT << 
        user.Name() << " says, \"" << *line << "\"." NEWLINE PLAINTEXT;

    user.GetLocation()->TellAllBut(user, out.str());

    interface << "You say, \"" << *line << "\" to those in the area." NEWLINE;
}
