#ifndef GRAMMAR_BUILDER_HPP
#define GRAMMAR_BUILDER_HPP

#include "Server/Ansi.hpp"
#include "Grammar.hpp"

namespace Mud
{
namespace Grammar
{

struct SuppressHelp_t {};
extern struct SuppressHelp_t SuppressHelp;

class GrammarBuilder
{
public:
    GrammarBuilder(Grammar &grammar, Dictionary::Dictionary &dictionary)
        : m_grammar(grammar),
          m_dictionary(dictionary),
          m_begin(grammar.m_grammars.size()), m_end(m_begin),
          m_suppressHelp(false),
          m_privileged(false)
        {}
    ~GrammarBuilder() { EndOfVerb(); }

    template <class T>
    void NewVerb(T &&verb)
    {
        NewVerb({std::forward<T>(verb)});
    }

    void NewVerb(std::vector<std::string> &&verbs);

    template <class ActionType>
    GrammarBuilder& NewGrammarLine()
    {
        EndOfGrammarLine();
        m_suppressHelp = false;
        NewGrammarLineInternal<ActionType>();
        return *this;
    }

    template <class ActionType>
    GrammarBuilder& NewGrammarLine(struct SuppressHelp_t)
    {
        EndOfGrammarLine();
        m_suppressHelp = true;
        NewGrammarLineInternal<ActionType>();
        return *this;
    }

    GrammarBuilder& ExactWord(const std::string &word);
    GrammarBuilder& OneOf(const std::vector<std::string> &words);
    GrammarBuilder& DirectObject();
    GrammarBuilder& IndirectObject();
        
private:
    template <class ActionType>
    void NewGrammarLineInternal()
    {
        ++m_end;
        m_lastGrammarLine = m_grammar.m_grammars.emplace_back(
            std::make_unique<GrammarLine<ActionType> >()
            ).get();

        auto grammar = m_grammar.m_grammars.rbegin();
        
        if (m_end - m_begin == 1 ||
            typeid(**grammar) != typeid(**(grammar+1)))
        {
            m_verbHelp << NEWLINE << ActionType::Description << ":" NEWLINE;
        }
        if (!m_suppressHelp)
        {
            m_verbHelp << "    " << m_verbs.front();
        }
    }

    void EndOfGrammarLine();
    void EndOfVerb();

    Grammar &m_grammar;
    GrammarLineBase *m_lastGrammarLine;
    Dictionary::Dictionary &m_dictionary;
    std::ostringstream m_verbHelp;
    std::vector<std::string> m_verbs;
    int m_begin, m_end;
    bool m_suppressHelp, m_privileged;
};

}
}

#endif
