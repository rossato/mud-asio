#ifndef MUDPARSER_HPP
#define MUDPARSER_HPP

namespace Mud
{
namespace Parser
{
template <class ContextType> class Parser;
}

namespace Interface
{
class MudInterface;

typedef Parser::Parser<MudInterface> MudParser;

//extern MudParser gMudParser;
}
}

#endif
