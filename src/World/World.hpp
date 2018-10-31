#ifndef WORLD_HPP
#define WORLD_HPP

#include <map>
#include "Location.hpp"
#include "User.hpp"

namespace Mud
{
namespace Dictionary
{
class Dictionary;
}
namespace Grammar
{
class Grammar;
}
namespace Server
{
class Server;
}

namespace World
{

class World
{
    friend class WorldBuilder;

public:
    World(Server::Server &server, Grammar::Grammar &grammar, Dictionary::Dictionary &dictionary);

    User *GetUser(const std::string &user);
    Location &GetLocation(int index)
    {
        return m_locations.at(index);
    }

    Server::Server &Server() { return m_server; }
    Grammar::Grammar &Grammar() { return m_grammar; }
    Dictionary::Dictionary &Dictionary() { return m_dictionary; }

    void TestInit();

private:
    std::map<int, Location> m_locations;
    std::map<std::string, User> m_userDatabase;

    Server::Server &m_server;
    Grammar::Grammar &m_grammar;
    Dictionary::Dictionary &m_dictionary;
};

}
}

#endif
