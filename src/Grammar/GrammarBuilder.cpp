#include "GrammarBuilder.hpp"
#include "Server/Ansi.hpp"

namespace Mud
{
namespace Grammar
{
struct SuppressHelp_t SuppressHelp;
    
void GrammarBuilder::NewVerb(std::vector<std::string> &&verbs)
{
    EndOfVerb();
    m_verbs = std::move(verbs);

    auto verb = m_verbs.begin();
    m_verbHelp << "Usage of \"" << *verb << "\"";

    for (++verb; verb != m_verbs.end(); ++verb)
    {
        m_verbHelp << "/\"" << *verb << "\"";
    }
    m_verbHelp << ":" NEWLINE;
}

GrammarBuilder& GrammarBuilder::ExactWord(const std::string &word)
{
    m_lastGrammarLine->EmplaceRule(m_dictionary.TryInsert(word, Dictionary::GRAMMAR));

    if (!m_suppressHelp) m_verbHelp << " " << word;
    return *this;
}

GrammarBuilder& GrammarBuilder::OneOf(const std::vector<std::string> &words)
{
    bool first = true;
    for (auto &word : words)
    {
        m_lastGrammarLine->EmplaceRule(m_dictionary.TryInsert(word, Dictionary::GRAMMAR));

        if (!m_suppressHelp)
        {
            if (first)
            {
                m_verbHelp << " ";
                first = false;
            }
            else
                m_verbHelp << "/";

            m_verbHelp << word;
        }
    }
    return *this;
}

GrammarBuilder& GrammarBuilder::DirectObject()
{
    m_lastGrammarLine->EmplaceRule(TokenType::DIRECT);
    if (!m_suppressHelp) m_verbHelp << " " << m_lastGrammarLine->DirectDescription();
    return *this;
}

GrammarBuilder& GrammarBuilder::IndirectObject()
{
    m_lastGrammarLine->EmplaceRule(TokenType::INDIRECT);
    if (!m_suppressHelp) m_verbHelp << " " << m_lastGrammarLine->IndirectDescription();
    return *this;
}
    
void GrammarBuilder::EndOfGrammarLine()
{
    if (m_begin != m_end && !m_suppressHelp) m_verbHelp << NEWLINE;
    m_suppressHelp = false;
}

void GrammarBuilder::EndOfVerb()
{
    EndOfGrammarLine();

    for (const auto &verb : m_verbs)
    {
        m_grammar.m_grammarIndex.emplace(verb, VerbInfo {
                privileged: m_privileged,
                    begin: m_begin, end: m_end,
                    help: m_verbHelp.str()
                    });
    }
    m_verbs.clear();
    m_verbHelp.str("");
    m_begin = m_end;
    m_privileged = false;
}

}
}
