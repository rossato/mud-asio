#ifndef ACTIONS_HPP
#define ACTIONS_HPP

namespace Mud
{
namespace Dictionary
{
class Dictionary;
}
namespace Grammar
{
class Grammar;
}

namespace Actions
{

void PopulateGrammarWithActions(Grammar::Grammar &, Dictionary::Dictionary &);

}
}

#endif
