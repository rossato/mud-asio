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

void Grammar::Parse(Dictionary::Tokenizer &tok) const
{
    const auto &verb = tok.GetString();
    GrammarIndexType::const_iterator verb_grammars = m_grammarIndex.find(verb);
    if (verb_grammars == m_grammarIndex.end())
    {
        std::ostringstream out;
        out << "\"" << verb << "\" is not a known command." NEWLINE;
        throw UsageException(out.str());
    }
    
    GrammarListType::const_iterator
        grammar = m_grammars.begin() + verb_grammars->second.begin,
        end     = m_grammars.begin() + verb_grammars->second.end;

    for (; grammar != end; ++grammar, tok.Seek(1))
    {
        if ((*grammar)->TryParse(tok)) return;
    }
    std::ostringstream out;
    out << "I didn't understand the usage of that command, try \"help " << verb << "\"." NEWLINE;
    throw UsageException(out.str());
}

}
}
