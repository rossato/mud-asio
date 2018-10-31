#ifndef ADMIN_VERBS_HPP
#define ADMIN_VERBS_HPP

#include "Grammar/BasicMatchers.hpp"
#include "Server/Ansi.hpp"
#include "Server/Server.hpp"
#include "World/World.hpp"

namespace Mud
{
namespace Logic
{

struct ShutdownAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Grammar::NumberMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response, DirectMatcher::ValueType seconds, int)
    {
        // todo call world cleanup
        // todo permission checking
        std::cout << "Server received request from " << user.Name() << " to shut down in "
                  << *seconds << " seconds." NEWLINE;
        response << "Server will shut down in " << *seconds << " seconds." NEWLINE;

//  This functionality requires a list of currently logged in users.
//  I don't want to do this through the Server connection pool because
//  that may be serving connections of different protocols.
//        user.World().Broadcast("Alert message");
        
        user.World().Server().Shutdown(*seconds);
    }
};

struct ShutdownHelpAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response, int, int)
    {
        response <<
            "When would you like to shutdown the server?" NEWLINE
            "(enter \"shutdown now\" to shutdown now, or \"help shutdown\" for more options)" NEWLINE;
    }
};

struct ShutdownNowAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response, int, int)
    {
        ShutdownAction::Act(user, response, 0, 0);
    }
};

struct ShutdownCancelAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response, int, int)
    {
        std::cout << "Server received request from " << user.Name() << " to cancel shut down." NEWLINE;

        user.World().Server().CancelShutdown();
        response << "Server shutdown cancelled." NEWLINE;
    }
};

}
}

#endif
