#ifndef LOGIC_HPP
#define LOGIC_HPP

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

namespace Logic
{

void PopulateGrammarWithLogic(Grammar::Grammar &, Dictionary::Dictionary &);

}
}

#endif
