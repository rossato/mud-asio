#ifndef HELP_ACTIONS_HPP
#define HELP_ACTIONS_HPP

#include <string>
#include "Parser/BasicMatchers.hpp"

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

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;
    
    static void Act(Interface::MudInterface &);
};

struct HelpAboutAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &);
};

struct HelpVerbAction
{
    static const std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::VerbMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &,
                    Parser::VerbMatcher::ValueType);
};

}
}

#endif
