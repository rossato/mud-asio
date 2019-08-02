#ifndef BASIC_ACTIONS_HPP
#define BASIC_ACTIONS_HPP

#include "Grammar/BasicMatchers.hpp"
#include "World/Direction.hpp"

namespace Mud
{
namespace Interface
{
class MudInterface;
}
namespace World
{
class User;
}

namespace Actions
{

struct QuitAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, int, int);
};

struct LookAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, int, int);
};

struct SayAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::RestOfLineMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, Grammar::RestOfLineMatcher::ValueType &line, int);
};

struct DirectionMatcher
{
    static const std::string Description;
    
    typedef World::Direction ValueType;
    typedef Dictionary::Tokenizer InterfaceType;
    
    static ValueType Match(InterfaceType &tok)
    {
        return World::TokenToDirection(tok.GetToken());
    }
};
    
struct GoAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef DirectionMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, DirectionMatcher::ValueType dir, int);
};

struct GoNowhereAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, int, int)
    {
        GoAction::Act(interface, World::NODIR, 0);
    }
};

template <World::Direction dir>
struct GoDirAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, int, int)
    {
        GoAction::Act(interface, dir, 0);
    }
};

template <World::Direction dir>
const std::string GoDirAction<dir>::Description("Travel in a specific direction");

}
}

#endif
