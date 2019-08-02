#ifndef NOUN_MATCH_EVALUATOR_HPP
#define NOUN_MATCH_EVALUATOR_HPP

#include "Dictionary/Tokenizer.hpp"
#include "World/User.hpp"
#include "World/Location.hpp"

namespace Mud
{
namespace Actions
{

extern const std::string CANT_SELF;
    
class NounMatchEvaluator
{
public:
    NounMatchEvaluator(Dictionary::Tokenizer &tok);

    constexpr bool IsSelf() const
    {
        return m_firstToken.IsSelf();
    }

    bool IsGrammatical() const;

    void Evaluate(World::Noun *noun);

    void EvaluateItemsInInventory(World::User &actor)
    {
        for (auto item : actor.GetInventory()) Evaluate(item);
    }

    void EvaluateItemsInLocation(World::Noun &location)
    {
        for (auto item : location.GetItems()) Evaluate(item);
    }

    void EvaluateUsersInLocation(World::Location &location)
    {
        for (auto user : location.GetUsers()) Evaluate(user);
    }

    World::Noun *BestCandidate() const;

    [[ noreturn ]] void ThrowMissingNounException() const;

private:
    Dictionary::Tokenizer &m_tokenizer;
    Dictionary::Token m_firstToken;

    int m_startPos, m_bestPos;
    World::Noun *m_bestCandidate;
    int m_nBestTokens;
    bool m_ambiguous;
};

}
}

#endif
