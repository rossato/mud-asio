#include "NounMatchEvaluator.hpp"

#include "Grammar/UsageException.hpp"
#include "World/Noun.hpp"

namespace Mud
{
namespace Actions
{

const std::string CANT_SELF("That's not an action you can participate in.");

NounMatchEvaluator::NounMatchEvaluator(Dictionary::Tokenizer &tok)
    : m_tokenizer(tok),
      m_bestCandidate(nullptr),
      m_nBestTokens(0), m_ambiguous(false)
{
    auto firstToken = tok.GetToken();
    for (; firstToken.IsArticle(); firstToken = tok.GetToken());
    m_firstToken = firstToken;

    tok.Unget();
    m_startPos = tok.Tell();
}

bool NounMatchEvaluator::IsGrammatical() const
{
    if (m_firstToken.IsSelf())
        throw Grammar::UsageException(CANT_SELF);

    if (!m_firstToken.IsNoun()) return false;

    if (m_firstToken == Dictionary::Dictionary::It && !m_tokenizer.It())
    {
        throw Grammar::UsageException("I don't know what \"it\" is.");
    }

    return true;
}
    
void NounMatchEvaluator::Evaluate(World::Noun *noun)
{
    m_tokenizer.Seek(m_startPos);
    int result = 0;
    auto token = m_tokenizer.GetToken();

    if (token == Dictionary::Dictionary::It)
    {
        if (noun == m_tokenizer.It())
        {
            m_bestPos = m_tokenizer.Tell();
            m_bestCandidate = noun;
        }
        return;
    }
        
    for (; token.IsNoun() && noun->IsReferredToAs(token)
             ; token = m_tokenizer.GetToken(), ++result);
    if (token) m_tokenizer.Unget();

    if (result && result == m_nBestTokens && noun != m_bestCandidate)
    {
        m_ambiguous = true;
    }
    else if (result > m_nBestTokens)
    {
        m_bestPos = m_tokenizer.Tell();
        m_bestCandidate = noun;
        m_nBestTokens = result;
        m_ambiguous = false;
    }
}

World::Noun *NounMatchEvaluator::BestCandidate() const
{
    if (m_ambiguous)
    {
        std::ostringstream error;
        error << "There are multiple items that match the description";
        m_tokenizer.Seek(m_startPos);
        for (int i = 0; i < m_nBestTokens; ++i)
        {
            error << " " << m_tokenizer.GetString();
        }
        throw Grammar::UsageException(error.str());
    }
    if (m_bestCandidate)
    {
        m_tokenizer.Seek(m_bestPos);
        m_tokenizer.SetIt(m_bestCandidate);
    }
    return m_bestCandidate;
}

[[ noreturn ]] void NounMatchEvaluator::ThrowMissingNounException() const
{
    std::ostringstream error;
    if (m_firstToken == Dictionary::Dictionary::It)
    {
        error << "I don't see " << m_tokenizer.It()->theName() << " here.";
    }
    else
    {    
        m_tokenizer.Seek(m_startPos);
        error << "I don't see any \"" << m_tokenizer.GetString() << "\" here.";
    }
    throw Grammar::UsageException(error.str());
}

}
}
