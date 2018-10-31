#include "ItemHandling.hpp"
#include "Grammar/UsageException.hpp"
#include "Server/Ansi.hpp"
#include "World/Location.hpp"
#include "World/Noun.hpp"
#include "World/User.hpp"
#include "NounMatchEvaluator.hpp"

using namespace Mud;
using namespace Mud::Logic;

std::string NounMatcher::Description("<noun>");
std::string HeldMatcher::Description("<held item>");

std::string    LookAtAction::Description("Examine/look at something within sight");
std::string      TakeAction::Description("Pick up an item in the current area");
std::string      DropAction::Description("Put down an item that you have");
std::string InventoryAction::Description("List the things that you have");

NounMatcher::ValueType NounMatcher::Match(World::User &actor, Dictionary::Tokenizer &tok)
{   
    NounMatchEvaluator nounEvaluator(tok);

    if (nounEvaluator.IsSelf()) return &actor;
    if (!nounEvaluator.IsGrammatical()) return nullptr;

    nounEvaluator.EvaluateItemsInInventory(actor);
    nounEvaluator.EvaluateItemsInLocation(actor.GetLocation());
    nounEvaluator.EvaluateUsersInLocation(actor.GetLocation());

    auto candidate = nounEvaluator.BestCandidate();
    if (candidate) return candidate;

    nounEvaluator.ThrowMissingNounException();
}

HeldMatcher::ValueType HeldMatcher::Match(World::User &actor, Dictionary::Tokenizer &tok)
{
    NounMatchEvaluator itemEvaluator(tok);

    if (!itemEvaluator.IsGrammatical()) return nullptr;

    itemEvaluator.EvaluateItemsInInventory(actor);
    
    auto candidate = itemEvaluator.BestCandidate();
    if (candidate) return candidate;

    itemEvaluator.EvaluateItemsInLocation(actor.GetLocation());

    if (candidate = itemEvaluator.BestCandidate())
    {
        // TODO Implicit take?
        
        std::ostringstream error;
        error << "You don't have " << candidate->theName() << ".";
        throw Grammar::UsageException(error.str());
    }

    itemEvaluator.EvaluateUsersInLocation(actor.GetLocation());

    if (candidate = itemEvaluator.BestCandidate())
    {
        if (candidate == &actor)
            throw Grammar::UsageException(CANT_SELF);
        
        std::ostringstream error;
        error << candidate->Name() << " wouldn't appreciate that very much.";
        throw Grammar::UsageException(error.str());
    }

    itemEvaluator.ThrowMissingNounException();
}
    
void LookAtAction::Act(World::User &user, std::ostream &response,
                       NounMatcher::ValueType noun, int)
{
    if (noun == &user)
    {
        response << "You don't ";
    }
    else
    {
        response << noun->TheName() << " doesn't ";
    }
    response << "seem special to me." NEWLINE;
}
    
void TakeAction::Act(World::User &user, std::ostream &response,
                     NounMatcher::ValueType item, int)
{
    if (item == &user)
    {
        response << CANT_SELF << NEWLINE;
        return;
    }
    if (item->IsAUser())
    {
        response << item->Name() << " wouldn't appreciate that very much." NEWLINE;
        return;
    }
    
    auto begin = user.GetLocation().GetItems().begin(),
        end = user.GetLocation().GetItems().end();
    
    if (std::find(begin, end, item) == end)
    {
        response << "You already have " << item->theName() << "." NEWLINE;
        return;
    }

    user.AddToInventory(*item);
    user.GetLocation().RemoveItem(*item);

    std::ostringstream out;
    out << user.Name() << " takes " << item->theName() << "." NEWLINE;
    user.GetLocation().TellAllBut(user, out.str());

    response << "You take " << item->theName() << "." NEWLINE;
}

void DropAction::Act(World::User &user, std::ostream &response,
                     HeldMatcher::ValueType item, int)
{
    user.GetLocation().AddItem(*item);
    user.RemoveFromInventory(*item);

    std::ostringstream out;
    out << user.Name() << " drops " << item->aName() << " here." NEWLINE;
    user.GetLocation().TellAllBut(user, out.str());

    response << "You drop " << item->theName() << "." NEWLINE;
}

void InventoryAction::Act(World::User &user, std::ostream &response, int, int)
{
    response << "You have the following items: " NEWLINE;

    for (auto item : user.GetInventory())
    {
        response << "  " << item->aName() << NEWLINE;
    }
}
