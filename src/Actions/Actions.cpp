#include "Actions.hpp"
#include "Grammar/GrammarBuilder.hpp"
#include "AdminActions.hpp"
#include "BasicActions.hpp"
#include "HelpActions.hpp"
#include "ItemHandling.hpp"

using namespace Mud::Grammar;
using namespace Mud::Actions;

namespace Mud
{
namespace Actions
{

void PopulateGrammarWithActions(Grammar::Grammar &grammar, Dictionary::Dictionary &dictionary)
{
    std::cout << "Initializing rules of grammar..." << std::endl;
    GrammarBuilder builder(grammar, dictionary);

    // HelpActions.hpp

    builder.NewVerb("help");
    builder.NewGrammarLine<HelpAction>();
    builder.NewGrammarLine<HelpVerbAction>()
        .DirectObject();
    builder.NewGrammarLine<HelpAboutAction>() // This is "shadow grammar". Shows up in help,
        .ExactWord("about");                  //  but is actually called by HelpVerbAction.

    // AdminActions.hpp

    builder.NewVerb("shutdown");
    builder.NewGrammarLine<ShutdownAction>()
        .DirectObject();
    builder.NewGrammarLine<ShutdownNowAction>()
        .ExactWord("now");
    builder.NewGrammarLine<ShutdownCancelAction>()
        .OneOf({"abort", "cancel"});

    builder.NewVerb("debug");
    builder.NewGrammarLine<DebugAction>();
    
    // BasicActions.hpp

    builder.NewVerb("quit");
    builder.NewGrammarLine<QuitAction>();

    builder.NewVerb({"look", "l"});
    builder.NewGrammarLine<LookAction>();
    builder.NewGrammarLine<LookAtAction>()
        .ExactWord("at")
        .DirectObject();

    builder.NewVerb("say");
    builder.NewGrammarLine<SayAction>()
        .DirectObject();

    builder.NewVerb("go");
    builder.NewGrammarLine<GoNowhereAction>(); // Encourage the user to specify a dir.
    builder.NewGrammarLine<GoAction>()
        .DirectObject();

    builder.NewVerb({"north", "n"});
    builder.NewGrammarLine<GoDirAction<World::N> >();
    builder.NewVerb({"east", "e"});
    builder.NewGrammarLine<GoDirAction<World::E> >();
    builder.NewVerb({"south", "s"});
    builder.NewGrammarLine<GoDirAction<World::S> >();
    builder.NewVerb({"west", "w"});
    builder.NewGrammarLine<GoDirAction<World::W> >();
    builder.NewVerb({"northeast", "ne"});
    builder.NewGrammarLine<GoDirAction<World::NE> >();
    builder.NewVerb({"southeast", "se"});
    builder.NewGrammarLine<GoDirAction<World::SE> >();
    builder.NewVerb({"southwest", "sw"});
    builder.NewGrammarLine<GoDirAction<World::SW> >();
    builder.NewVerb({"northwest", "nw"});
    builder.NewGrammarLine<GoDirAction<World::NW> >();
    builder.NewVerb({"up", "u"});
    builder.NewGrammarLine<GoDirAction<World::U> >();
    builder.NewVerb({"down", "d"});
    builder.NewGrammarLine<GoDirAction<World::D> >();
    builder.NewVerb("in");
    builder.NewGrammarLine<GoDirAction<World::IN> >();
    builder.NewVerb("out");
    builder.NewGrammarLine<GoDirAction<World::OUT> >();

    // ItemHandling.hpp

    builder.NewVerb({"examine", "x"});
    builder.NewGrammarLine<LookAtAction>()
        .DirectObject();

    builder.NewVerb({"take", "get"});
    builder.NewGrammarLine<TakeAction>()
        .DirectObject();

    builder.NewVerb("pick");
    builder.NewGrammarLine<TakeAction>()
        .ExactWord("up")
        .DirectObject();
    builder.NewGrammarLine<TakeAction>()
        .DirectObject()
        .ExactWord("up");

    builder.NewVerb("drop");
    builder.NewGrammarLine<DropAction>()
        .DirectObject();

    builder.NewVerb("put");
    builder.NewGrammarLine<DropAction>()
        .ExactWord("down")
        .DirectObject();
    builder.NewGrammarLine<DropAction>()
        .DirectObject()
        .ExactWord("down");

    builder.NewVerb({"inventory", "inv"});
    builder.NewGrammarLine<InventoryAction>();
}

}
}
