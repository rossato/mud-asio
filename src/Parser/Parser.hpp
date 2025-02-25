#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Dictionary/Dictionary.hpp"
#include "Dictionary/Token.hpp"
#include "Dictionary/Tokenizer.hpp"
#include "Server/Ansi.hpp"
#include "Grammar.hpp"
#include "UsageException.hpp"

namespace Mud
{
namespace World
{
class User;
}
namespace Parser
{

class VerbInfo
{
public:
    bool privileged;
    int begin, end;
    std::string help;
};
typedef std::map<std::string, VerbInfo> VerbIndex;

template <class ContextType> class GrammarBuilder;

template <class ContextType>
class Parser
{
    friend class GrammarBuilder<ContextType>;

public:

    // I fiercely debated whether to return an error, write to the "context", or throw UsageExceptions.
    // I'm going with exceptions because:
    //  - Exceptions are conventional in C++.
    //  - Downstream classes (matchers, actions) need to abort processing.
    //  - WordNotFoundException already exists and is MUCH harder to get rid of.
    //
    // The downside is that exceptions could be thrown much more frequently than typical
    //  because they're caused by bad user input.  This isn't so much an "error" as an expected
    //  outcome of bad input.  Nonetheless because of the need downstream to abort processing,
    //  and because exceptions are the conventional way of doing this sort of thing, here we go.
    
    void Parse(ContextType &c, Dictionary::Tokenizer &tokenizer)
    {
        const auto &verb = tokenizer.GetString();
        VerbIndex::const_iterator verb_grammars = m_grammarIndex.find(verb);
        if (verb_grammars == m_grammarIndex.end())
        {
            std::ostringstream out;
            out << "\"" << verb << "\" is not a known command." NEWLINE;
            throw UsageException(out.str());
        }

        typename GrammarListType::const_iterator
            grammar = m_grammars.begin() + verb_grammars->second.begin,
            end     = m_grammars.begin() + verb_grammars->second.end;
        
        for (; grammar != end; ++grammar, tokenizer.Seek(1))
        {
            if ((*grammar)->TryParse(c, tokenizer)) return;
        }
        std::ostringstream out;
        out << "I didn't understand the usage of that command, try \"help " << verb << "\"." NEWLINE;
        throw UsageException(out.str());
    }

    using GrammarListType = std::vector<std::unique_ptr<GrammarBase<ContextType> > >;

    const GrammarListType &Grammars() const { return m_grammars; }
    const VerbIndex &GrammarIndex() const { return m_grammarIndex; }

private:
    GrammarListType m_grammars;
    VerbIndex m_grammarIndex;
};

}
}

#endif
