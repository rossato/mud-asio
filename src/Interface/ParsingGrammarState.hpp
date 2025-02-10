#ifndef PARSING_GRAMMAR_STATE_HPP
#define PARSING_GRAMMAR_STATE_HPP

#include "InterfaceStateBase.hpp"
#include "MudParser.hpp"

namespace Mud
{
namespace Dictionary
{
class Tokenizer;
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
    MudParser &m_parser;
    Dictionary::Tokenizer &m_tokenizer;
};

}
}

#endif
