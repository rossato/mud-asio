#ifndef WORLD_HPP
#define WORLD_HPP

#include <list>
#include <map>
#include "Location.hpp"
#include "Lua.hpp"
#include "User.hpp"

namespace Mud
{
namespace Dictionary
{
class Dictionary;
}
    
namespace World
{

class World
{
    friend class WorldBuilder;

public:
    World(Dictionary::Dictionary &dictionary)
        : m_dictionary(dictionary), m_lua(*this) {}

    User *GetUser(const std::string &user);
    void ReapUser(User &);

    Noun &GetLocation(int index)
    {
        return m_locations.at(index);
    }

    Dictionary::Dictionary &Dictionary() { return m_dictionary; }
    Mud::World::Lua &Lua() { return m_lua; }
    
    void TestInit();

    template <class... Args>
    Noun &CreateNoun(Args &&... args)
    {
        return m_nounRegistry.emplace_back(std::forward<Args>(args)...);
    }

    template <class... Args>
    Noun &CreateLocation(int id, Args &&... args)
    {
        return m_locations.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(id),
                                   std::forward_as_tuple(std::forward<Args>(args)...)).first->second;
    }
    
    Noun &DefaultSpawnPoint()
    {
        return m_locations.begin()->second;
    }

    void FinalizeLocations();
    
private:
    std::list<Noun> m_nounRegistry;
    std::map<int, Noun> m_locations;
    std::map<std::string, User> m_userCache;

    Dictionary::Dictionary &m_dictionary;
    Mud::World::Lua m_lua;
};

}
}

#endif
