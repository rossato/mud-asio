#include "ParsingGrammarState.hpp"

#include "Actions/BasicActions.hpp"
#include "Parser/Parser.hpp"
#include "Parser/UsageException.hpp"
#include "Server/Ansi.hpp"
#include "MudInterface.hpp"

using namespace Mud::Interface;

ParsingGrammarState::ParsingGrammarState(MudInterface &interface)
    : InterfaceStateBase(interface), m_parser(interface.Parser()), m_tokenizer(interface.Tokenizer())
{
    Mud::Actions::LookAction::Act(m_interface);
}

void ParsingGrammarState::HandleLine()
{
    try
    {
        m_parser.Parse(m_interface, m_tokenizer);
    }
    catch (const Dictionary::WordNotFoundException& e)
    {
        m_interface << "Parser error: I don't know what the word \"" << e.word() << "\" means." NEWLINE;
    }
    catch (const Parser::UsageException &e)
    {
        m_interface << "Parser error: " << e.what() << NEWLINE;
    }
}

void ParsingGrammarState::Prompt()
{
    m_interface.Write("> ");
}
