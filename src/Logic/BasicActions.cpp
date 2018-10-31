#include "BasicActions.hpp"

#include <iomanip>
#include "Server/Ansi.hpp"
#include "Grammar/Grammar.hpp"
#include "World/Location.hpp"

using namespace Mud::Logic;

const std::string  DirectionMatcher::Description("<direction>");
    
const std::string      LookAction::Description("Look around the current area");
const std::string      QuitAction::Description("Quit the MUD");
const std::string        GoAction::Description("Travel in a direction");
const std::string       SayAction::Description("Speak a line of text to those in the same area as you");

void QuitAction::Act(World::User &, std::ostream &response, int, int)
{
    response << "Goodbye!" NEWLINE;
    throw Server::CloseException();
}

void LookAction::Act(World::User &user, std::ostream &response, int, int)
{
    response << user.GetLocation();
    for (const auto otherUser : user.GetLocation().GetUsers())
    {
        if (&*otherUser != &user)
        {
            // TODO combine the users present into a single grammatic sentence.
            response << GREENTEXT << otherUser->Name() << " is here." PLAINTEXT NEWLINE;
        }
    }
    for (const auto item : user.GetLocation().GetItems())
    {
        response << "There is " << item->aName() << " here." NEWLINE;
    }
}

void GoAction::Act(World::User &user, std::ostream &response, World::Direction dir, int)
{
    if (dir == World::NODIR)
    {
        response << "Go where?" NEWLINE;
        return;
    }

    if (World::Location *location = user.GetLocation().GetExit(dir))
    {
        user.GetLocation().UserLeaving(user, dir);
        user.SetLocation(location);

        response << "You leave to " << dir << "." NEWLINE;
        LookAction::Act(user, response, 0, 0);

        location->UserArriving(user);
    }
    else
    {
        response << "There's nothing in that direction." NEWLINE;
    }
}

void SayAction::Act(World::User &user, std::ostream &response,
                    Grammar::RestOfLineMatcher::ValueType &line, int)
{
    if (line->empty())
    {
        response << "Say what?" NEWLINE;
        return;
    }

    std::ostringstream out;
    out << MAGENTATEXT << 
        user.Name() << " says, \"" << *line << "\"." NEWLINE PLAINTEXT;

    user.GetLocation().TellAllBut(user, out.str());

    response << "You say, \"" << *line << "\" to those in the area." NEWLINE;
}
