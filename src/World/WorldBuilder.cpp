#include "WorldBuilder.hpp"
#include "World.hpp"

using namespace Mud::World;

WorldBuilder::~WorldBuilder()
{
    for (auto &location : m_world.m_locations)
    {
        for (const auto index : location.second.m_exitIndexes)
        {
            location.second.m_exits[index.first] = &m_world.GetLocation(index.second);
        }
        location.second.Repop();
    }
}
    
Location &WorldBuilder::NewLocation(int index, const std::string &name)
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
    return m_lastLocation->EmplaceItem(words.front(), std::move(tokens));
}

User &WorldBuilder::NewUser(const std::string &name, int location)
{
    std::string downcased;
    std::transform(name.begin(), name.end(), std::back_inserter(downcased),
                   ::tolower);
    return m_world.m_userDatabase.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(downcased),
        std::forward_as_tuple(m_world, name, m_world.GetLocation(location))).first->second;
}
