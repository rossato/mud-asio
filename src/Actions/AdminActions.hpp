#ifndef ADMIN_VERBS_HPP
#define ADMIN_VERBS_HPP

#include "Interface/MudInterface.hpp"
#include "Parser/BasicMatchers.hpp"
#include "Server/Ansi.hpp"
#include "Server/Server.hpp"
#include "World/World.hpp"

namespace Mud
{
namespace Actions
{

struct ShutdownAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Parser::NumberMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, DirectMatcher::ValueType seconds)
    {
        // todo call world cleanup
        // todo permission checking
        std::cout << "Server received request from " << interface.User().Name() << " to shut down in "
                  << *seconds << " seconds." NEWLINE;
        interface << "Server will shut down in " << *seconds << " seconds." NEWLINE;

//  This functionality requires a list of currently logged in users.
//  I don't want to do this through the Server connection pool because
//  that may be serving connections of different protocols.
//        user.World().Broadcast("Alert message");
        
        interface.Server().Shutdown(*seconds);
    }
};

struct ShutdownHelpAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface)
    {
        interface.Write(
            "When would you like to shutdown the server?" NEWLINE
            "(enter \"shutdown now\" to shutdown now, or \"help shutdown\" for more options)" NEWLINE
            );
    }
};

struct ShutdownNowAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface)
    {
        ShutdownAction::Act(interface, 0);
    }
};

struct ShutdownCancelAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface)
    {
        std::cout << "Server received request from " << interface.User().Name() << " to cancel shut down." NEWLINE;

        interface.Server().CancelShutdown();
        interface.Write("Server shutdown cancelled." NEWLINE);
    }
};

struct DebugAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = true;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface);
};
    
}
}

#endif
