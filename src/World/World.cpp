#include "World.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include "Dictionary/Tokenizer.hpp"
#include "Server/Ansi.hpp"
#include "Server/Server.hpp"
#include "User.hpp"
#include "UserReader.hpp"
#include "WorldBuilder.hpp"
#include "WorldReader.hpp"

using namespace Mud::World;

void World::TestInit()
{
    std::cout << "Initializing MUD world with test data..." << std::endl;

    m_nounRegistry.clear();
    m_userCache.clear();
    m_locations.clear();

    WorldBuilder builder(*this);
    builder.NewLocation(1000, "Place")
        .SetExit(Direction::E, 1001)
        .WithDescription(
            "Greetings from beautiful Place! You are in a location" NEWLINE
            "that has fantastic vistas and a long description that" NEWLINE
            "goes on for three whole lines."
            );
    builder.NewItem({"book"});

    builder.NewLocation(1001, "Other Place")
        .SetExit(Direction::W, 1000)
        .WithDescription(
            "You found the other place!"
            );

    FinalizeLocations();

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
    std::map<std::string, User>::iterator it = m_userCache.find(userName);

    if (it != m_userCache.end())
    {
        std::cerr << "User " << it->second.Name() << " found in cache." << std::endl;
        return &it->second;
    }
    std::cerr << "User " << userName << " not found in cache, checking files." << std::endl;

    try
    {
        UserReader reader(*this);
        // Todo clean this up with boost filesystem
        std::string path("users/");
        path += userName + ".txt";
        std::ifstream infile(path);
        reader.Read(infile);
        return reader.CurrentUser();
    }
    catch(const std::exception &e)
    {
        std::cerr << "World::GetUser caught exception " << e.what() << std::endl;
    }

    return nullptr;
}

void World::ReapUser(User &user)
{
    std::string downcased;
    std::transform(user.Name().begin(), user.Name().end(),
                   std::back_inserter(downcased), ::tolower);
    m_userCache.erase(downcased);
}

void World::FinalizeLocations()
{
    for (auto &location : m_locations)
    {
        location.second.m_exits.clear();
        for (const auto index : location.second.m_exitIndexes)
        {
            location.second.m_exits[index.first] = &GetLocation(index.second);
        }
//        location.second.Repop();
    }
}
