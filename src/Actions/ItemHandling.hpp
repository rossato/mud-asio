#ifndef ITEM_HANDLING_HPP
#define ITEM_HANDLING_HPP

#include "Parser/BasicMatchers.hpp"

namespace Mud
{
namespace Interface
{
class MudInterface;
}
namespace World
{
class Noun;
}

namespace Actions
{
// Actions related to the movement of objects to/from players, locations etc

struct NounMatcher
{
    static std::string Description;

    typedef World::Noun* ValueType;

    static ValueType Match(Interface::MudInterface &interface, Dictionary::Tokenizer &tokenizer);
};
 
struct HeldMatcher
{
    static std::string Description;

    typedef World::Noun* ValueType;

    static ValueType Match(Interface::MudInterface &interface, Dictionary::Tokenizer &tokenizer);
};
    
struct LookAtAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef NounMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &interface, NounMatcher::ValueType noun);
};

struct TakeAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef NounMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &interface, NounMatcher::ValueType noun);
};

struct DropAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef HeldMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &interface, HeldMatcher::ValueType noun);
};
    
struct InventoryAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Parser::NoneMatcher DirectMatcher;
    typedef Parser::NoneMatcher IndirectMatcher;

    static void Act(Interface::MudInterface &interface);
};

}
}

#endif
