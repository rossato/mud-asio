#ifndef BASIC_ACTIONS_HPP
#define BASIC_ACTIONS_HPP

#include "Grammar/BasicMatchers.hpp"
#include "Server/CloseException.hpp"
#include "World/Direction.hpp"

namespace Mud
{
namespace World
{
class User;
}

namespace Logic
{

struct QuitAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    [[ noreturn ]] static void Act(World::User &, std::ostream &response, int, int);
};

struct LookAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &, std::ostream &response, int, int);
};

struct SayAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::RestOfLineMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &, std::ostream &response, Grammar::RestOfLineMatcher::ValueType &line, int);
};

struct DirectionMatcher
{
    static const std::string Description;
    
    typedef World::Direction ValueType;

    static ValueType Match(World::User &, Dictionary::Tokenizer &tok)
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

    static void Act(World::User &, std::ostream &response, DirectionMatcher::ValueType dir, int);
};

template <World::Direction dir>
struct GoDirAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response, int, int)
    {
        GoAction::Act(user, response, dir, 0);
    }
};

template <World::Direction dir>
const std::string GoDirAction<dir>::Description("Travel in a specific direction");

}
}

#endif
