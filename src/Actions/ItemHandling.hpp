#ifndef ITEM_HANDLING_HPP
#define ITEM_HANDLING_HPP

#include "Grammar/BasicMatchers.hpp"

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
    typedef Interface::MudInterface InterfaceType;

    static ValueType Match(InterfaceType &interface);
};
 
struct HeldMatcher
{
    static std::string Description;

    typedef World::Noun* ValueType;
    typedef Interface::MudInterface InterfaceType;

    static ValueType Match(InterfaceType &interface);
};
    
struct LookAtAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef NounMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface,
                    NounMatcher::ValueType noun, int);
};

struct TakeAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef NounMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface,
                    NounMatcher::ValueType noun, int);
};

struct DropAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef HeldMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface,
                    HeldMatcher::ValueType noun, int);
};
    
struct InventoryAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;
    typedef Interface::MudInterface InterfaceType;

    static void Act(InterfaceType &interface, int, int);
};

}
}

#endif
