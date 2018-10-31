#include "Grammar.hpp"

#include <iostream>
#include <typeinfo>
#include "Dictionary/Tokenizer.hpp"
#include "Server/Ansi.hpp"
#include "UsageException.hpp"

namespace Mud
{
namespace Grammar
{

void Grammar::Parse(World::User &speaker, std::ostream &response, Dictionary::Tokenizer &tok) const
{
    const auto &verb = tok.GetString();
    GrammarIndexType::const_iterator verb_grammars = m_grammarIndex.find(verb);
    if (verb_grammars == m_grammarIndex.end())
    {
        response << "Parser error: \"" << verb
                 << "\" is not a known command." NEWLINE;
        return;
    }
    
    try
    {
        GrammarListType::const_iterator
            grammar = m_grammars.begin() + verb_grammars->second.begin,
            end     = m_grammars.begin() + verb_grammars->second.end;

        for (; grammar != end; ++grammar, tok.Seek(1))
        {
            if ((*grammar)->TryParse(speaker, response, tok)) return;
        }
        response << "Parser error: I didn't understand the usage of that command, try \"help "
                 << verb << "\"." NEWLINE;
    }
    catch (const Dictionary::WordNotFoundException& e)
    {
        response << "Parser error: I don't know what the word \"" << e.word() << "\" means." NEWLINE;
    }
    catch (const UsageException &e)
    {
        response << "Parser error: " << e.what() << NEWLINE;
    }
}

}
}
