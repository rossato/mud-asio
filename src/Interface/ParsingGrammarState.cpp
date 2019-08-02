#include "ParsingGrammarState.hpp"

#include "Actions/BasicActions.hpp"
#include "Grammar/Grammar.hpp"
#include "Grammar/UsageException.hpp"
#include "Server/Ansi.hpp"
#include "MudInterface.hpp"

using namespace Mud::Interface;

ParsingGrammarState::ParsingGrammarState(MudInterface &interface)
        : InterfaceStateBase(interface), m_grammar(interface.Grammar())
{
    Mud::Actions::LookAction::Act(m_interface, 0, 0);
}

void ParsingGrammarState::HandleLine()
{
    try
    {
        m_grammar.Parse(m_interface);
    }
    catch (const Dictionary::WordNotFoundException& e)
    {
        m_interface << "Parser error: I don't know what the word \"" << e.word() << "\" means." NEWLINE;
    }
    catch (const Grammar::UsageException &e)
    {
        m_interface << "Parser error: " << e.what() << NEWLINE;
    }

}

void ParsingGrammarState::Prompt()
{
    m_interface.Write("> ");
}
