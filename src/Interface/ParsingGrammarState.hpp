#ifndef PARSING_GRAMMAR_STATE_HPP
#define PARSING_GRAMMAR_STATE_HPP

#include "InterfaceStateBase.hpp"

namespace Mud
{
namespace Grammar
{
class Grammar;
}

namespace Interface
{

class ParsingGrammarState : public InterfaceStateBase
{
public:
    ParsingGrammarState(MudInterface &interface);

    virtual void HandleLine() override;
    virtual void Prompt() override;

private:
    Grammar::Grammar &m_grammar;
};

}
}

#endif
