#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Dictionary/Dictionary.hpp"
#include "Dictionary/Token.hpp"
#include "Dictionary/Tokenizer.hpp"
#include "GrammarLine.hpp"

namespace Mud
{
namespace World
{
class User;
}
namespace Grammar
{
class VerbInfo
{
public:
    bool privileged;
    int begin, end;
    std::string help;
};
    
class Grammar
{
    friend class GrammarBuilder;

public:
    void Parse(Dictionary::Tokenizer &tok) const;
    
    using GrammarListType = std::vector<std::unique_ptr<GrammarLineBase> >;
    using GrammarIndexType = std::map<std::string, VerbInfo>;

    const GrammarListType &Grammars() const { return m_grammars; }
    const GrammarIndexType &GrammarIndex() const { return m_grammarIndex; }

private:
    GrammarListType m_grammars;
    GrammarIndexType m_grammarIndex;
};

}
}

#endif
