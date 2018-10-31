#include "World.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include "Dictionary/Tokenizer.hpp"
#include "Server/Ansi.hpp"
#include "World/User.hpp"
#include "WorldBuilder.hpp"
#include "WorldReader.hpp"

using namespace Mud::World;

World::World(Mud::Server::Server &server,
             Mud::Grammar::Grammar &grammar,
             Mud::Dictionary::Dictionary &dictionary)
    : m_server(server), m_grammar(grammar), m_dictionary(dictionary)
{
    std::cout << "Initializing MUD world..." << std::endl;

    std::ifstream infile("data/world.txt");
    WorldReader(*this).Read(infile);
}

void World::TestInit()
{
    std::cout << "Initializing MUD world with test data..." << std::endl;

    m_userDatabase.clear();
    m_locations.clear();

    WorldBuilder builder(*this);
    builder.NewLocation(1000, "Place")
        .SetDescription(
            "Greetings from beautiful Place! You are in a location" NEWLINE
            "that has fantastic vistas and a long description that" NEWLINE
            "goes on for three whole lines."
            )
        .SetExit(Direction::E, 1001);
    builder.NewItem({"book"});

    builder.NewLocation(1001, "Other Place")
        .SetDescription(
            "You found the other place!"
            )
        .SetExit(Direction::W, 1000);

    builder.NewUser("Ken", 1000);
    builder.NewUser("Paul", 1001);
}

namespace
{
    enum class ReaderState
    {
        BETWEEN_ROOMS = 0,
        READING_DESCRIPTION,
        READING_EXITS,
        READING_ITEMS
    };
    const std::string EOR("eor");
}

User* World::GetUser(const std::string &userName)
{
    std::map<std::string, User>::iterator it = m_userDatabase.find(userName);

    if (it != m_userDatabase.end())
    {
        return &it->second;
    }
    return nullptr;
}
