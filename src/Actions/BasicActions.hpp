#ifndef BASIC_ACTIONS_HPP
#define BASIC_ACTIONS_HPP

#include "Parser/BasicMatchers.hpp"
#include "World/Direction.hpp"

namespace Mud
{
namespace Interface
{
class MudInterface;
}

namespace Actions
{

struct QuitAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    
    static void Act(Interface::MudInterface &);
};

struct LookAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    
    static void Act(Interface::MudInterface &);
};

struct SayAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::RestOfLineMatcher DirectMatcher;
    typedef Parser::NoneMatcher       IndirectMatcher;

    static void Act(Interface::MudInterface &, const typename DirectMatcher::ValueType &);
};

struct DirectionMatcher
{
    static const std::string Description;
    
    typedef World::Direction ValueType;

    template <class ContextType>
    static ValueType Match(ContextType &, Dictionary::Tokenizer &tokenizer)
    {
        return World::TokenToDirection(tokenizer.GetToken());
    }
};

struct GoAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef DirectionMatcher     DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &, typename DirectionMatcher::ValueType);
};

struct GoNowhereAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &interface)
    {
        GoAction::Act(interface, World::NODIR);
    }
};

template <World::Direction dir>
struct GoDirAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &interface)
    {
        GoAction::Act(interface, dir);
    }
};

template <World::Direction dir>
const std::string GoDirAction<dir>::Description("Travel in a specific direction");

}
}

#endif
