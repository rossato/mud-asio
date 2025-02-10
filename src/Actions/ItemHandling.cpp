#include "ItemHandling.hpp"
#include "Interface/MudInterface.hpp"
#include "Parser/UsageException.hpp"
#include "Server/Ansi.hpp"
#include "World/Location.hpp"
#include "World/Noun.hpp"
#include "World/User.hpp"
#include "NounMatchEvaluator.hpp"

using namespace Mud;
using namespace Mud::Actions;

std::string NounMatcher::Description("<noun>");
std::string HeldMatcher::Description("<held item>");

std::string    LookAtAction::Description("Examine/look at something within sight");
std::string      TakeAction::Description("Pick up an item in the current area");
std::string      DropAction::Description("Put down an item that you have");
std::string InventoryAction::Description("List the things that you have");

// TODO Match via Scopes?

NounMatcher::ValueType NounMatcher::Match(Interface::MudInterface &interface, Dictionary::Tokenizer &tokenizer)
{   
    NounMatchEvaluator nounEvaluator(tokenizer);

    auto &actor = interface.User();
    
    if (nounEvaluator.IsSelf()) return &actor;
    if (!nounEvaluator.IsGrammatical()) return nullptr;

    nounEvaluator.EvaluateItemsInInventory(actor);
    nounEvaluator.EvaluateItemsInLocation(*actor.GetLocation());
    nounEvaluator.EvaluateUsersInLocation(*actor.GetLocation());

    auto candidate = nounEvaluator.BestCandidate();
    if (candidate) return candidate;

    nounEvaluator.ThrowMissingNounException();
}

HeldMatcher::ValueType HeldMatcher::Match(Interface::MudInterface &interface, Dictionary::Tokenizer &tokenizer)
{
    NounMatchEvaluator itemEvaluator(tokenizer);

    if (!itemEvaluator.IsGrammatical()) return nullptr;

    auto &actor = interface.User();
    itemEvaluator.EvaluateItemsInInventory(actor);
    
    auto candidate = itemEvaluator.BestCandidate();
    if (candidate) return candidate;

    itemEvaluator.EvaluateItemsInLocation(*actor.GetLocation());

    if ((candidate = itemEvaluator.BestCandidate()))
    {
        // TODO Implicit take?
        
        std::ostringstream error;
        error << "You don't have " << candidate->theName() << ".";
        throw Parser::UsageException(error.str());
    }

    itemEvaluator.EvaluateUsersInLocation(*actor.GetLocation());

    if ((candidate = itemEvaluator.BestCandidate()))
    {
        if (candidate == &actor)
            throw Parser::UsageException(CANT_SELF);
        
        std::ostringstream error;
        error << candidate->Name() << " wouldn't appreciate that very much.";
        throw Parser::UsageException(error.str());
    }

    itemEvaluator.ThrowMissingNounException();
}

// TODO these Acts should all be in Lua

void LookAtAction::Act(Interface::MudInterface &interface, NounMatcher::ValueType noun)
{
    if (noun == &interface.User())
    {
        interface.Write("You take a long hard look at yourself." NEWLINE);
    }
    else
    {
        interface << noun->Description() << NEWLINE;
    }
}
    
void TakeAction::Act(Interface::MudInterface &interface, NounMatcher::ValueType item)
{
    auto &user = interface.User();
    if (item == &user)
    {
        interface << CANT_SELF << NEWLINE;
        return;
    }
    if (item->IsAUser())
    {
        interface << item->Name() << " wouldn't appreciate that very much." NEWLINE;
        return;
    }
    
    auto begin = user.GetLocation()->GetItems().begin(),
        end = user.GetLocation()->GetItems().end();
    
    if (std::find(begin, end, item) == end)
    {
        interface << "You already have " << item->theName() << "." NEWLINE;
        return;
    }

//    if (item->ReactToTake(user, interface.ostream())) return;

    user.AddToInventory(*item);
    user.GetLocation()->RemoveItem(*item);

    std::ostringstream out;
    out << user.Name() << " takes " << item->theName() << "." NEWLINE;
    user.GetLocation()->TellAllBut(user, out.str());

    interface << "You take " << item->theName() << "." NEWLINE;
}

void DropAction::Act(Interface::MudInterface &interface, HeldMatcher::ValueType item)
{
    auto &user = interface.User();
    user.GetLocation()->AddItem(*item);
    user.RemoveFromInventory(*item);

    std::ostringstream out;
    out << user.Name() << " drops " << item->aName() << " here." NEWLINE;
    user.GetLocation()->TellAllBut(user, out.str());

    interface << "You drop " << item->theName() << "." NEWLINE;
}

void InventoryAction::Act(Interface::MudInterface &interface)
{
    interface.Write("You have the following items: " NEWLINE);

    for (auto item : interface.User().GetInventory())
    {
        interface << "  " << item->aName() << NEWLINE;
    }
}
