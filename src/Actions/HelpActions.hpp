#ifndef HELP_ACTIONS_HPP
#define HELP_ACTIONS_HPP

#include <string>
#include "Grammar/BasicMatchers.hpp"

namespace Mud
{
namespace Interface
{
class MudInterface;
}

namespace Actions
{

struct HelpAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;
    
    static void Act(InterfaceType &, int, int);
};

struct HelpAboutAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &, int, int);
};

struct HelpVerbAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::VerbMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &,
                    Grammar::VerbMatcher::ValueType, int);
};

}
}

#endif
