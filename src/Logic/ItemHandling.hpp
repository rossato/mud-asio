#ifndef ITEM_HANDLING_HPP
#define ITEM_HANDLING_HPP

#include "BasicActions.hpp"

namespace Mud
{
namespace Dictionary
{
class Tokenizer;
}
namespace World
{
class Noun;
class User;
}

namespace Logic
{
// Actions related to the movement of objects to/from players, locations etc

struct NounMatcher
{
    static std::string Description;

    typedef World::Noun* ValueType;
    static ValueType Match(World::User &actor, Dictionary::Tokenizer &tok);
};
 
struct HeldMatcher
{
    static std::string Description;

    typedef World::Noun* ValueType;
    static ValueType Match(World::User &actor, Dictionary::Tokenizer &tok);
};
    
struct LookAtAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef NounMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response,
                    NounMatcher::ValueType noun, int);
};

struct TakeAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef NounMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response,
                    NounMatcher::ValueType noun, int);
};

struct DropAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef HeldMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response,
                    HeldMatcher::ValueType noun, int);
};
    
struct InventoryAction
{
    static std::string Description;
    static const bool RequiresPrivilege = false;

    typedef Grammar::NoneMatcher DirectMatcher;
    typedef Grammar::NoneMatcher IndirectMatcher;

    static void Act(World::User &user, std::ostream &response, int, int);
};

}
}

#endif
