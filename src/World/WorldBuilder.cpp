#include "WorldBuilder.hpp"

#include "Dictionary/Dictionary.hpp"
#include "World.hpp"

using namespace Mud::World;

Noun &WorldBuilder::NewLocation(int index, const std::string &name)
{
    m_lastLocation =
        &(m_world.m_locations.emplace(
              std::piecewise_construct,
              std::forward_as_tuple(index),
              std::forward_as_tuple(
                  name,
                  std::vector<Dictionary::Token>{m_world.Dictionary().TryInsert(name, Dictionary::NOUN)}
                  )
              ).first->second);
    return *m_lastLocation;
}

Noun &WorldBuilder::NewItem(const std::vector<std::string> &words)
{
    std::vector<Dictionary::Token> tokens;
    std::transform(words.begin(), words.end(), std::back_inserter(tokens),
                   [this](const std::string &str)
                   {
                       return m_world.Dictionary().TryInsert(str, Dictionary::NOUN);
                   });
    auto &item = m_world.CreateNoun(words.front(), std::move(tokens));
    m_lastLocation->AddItem(item);
    return item;
}

User &WorldBuilder::NewUser(const std::string &name, int location)
{
    std::string downcased;
    std::transform(name.begin(), name.end(), std::back_inserter(downcased),
                   ::tolower);
    return m_world.m_userCache.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(downcased),
        std::forward_as_tuple(m_world, name, m_world.GetLocation(location))).first->second;
}

