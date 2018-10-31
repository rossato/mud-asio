#ifndef HELP_ACTIONS_HPP
#define HELP_ACTIONS_HPP

#include <string>
#include "Grammar/BasicMatchers.hpp"

namespace Mud
{
namespace World
{
class User;
}

namespace Logic
{

struct HelpAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &, std::ostream &response, int, int);
};

struct HelpAboutAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &, std::ostream &response, int, int);
};

struct HelpVerbAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::VerbMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &, std::ostream &response,
                    Grammar::VerbMatcher::ValueType, int);
};

}
}

#endif
