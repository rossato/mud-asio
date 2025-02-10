#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "Interface/MudParser.hpp"

namespace Mud
{
namespace Dictionary
{
class Dictionary;
}
namespace Actions
{

void PopulateParserWithActions(Interface::MudParser &, Dictionary::Dictionary &);

}
}

#endif
