#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include "Dictionary/Tokenizer.hpp"

namespace Mud
{
namespace Grammar
{
class Grammar;
}
namespace Server
{
class ConnectionBase;
}
namespace World
{
class User;
class World;
}

namespace Logic
{

class MudInterface {
public:
    MudInterface(Server::ConnectionBase &connection, World::World &world);
    ~MudInterface();
    
    void HandleLine(const std::string &line);

private:
    void DisplayPrompt();
    void HandleLoginPrompt();
    void HandlePassPrompt();
    void HandleOrdinaryLine();

    Server::ConnectionBase &m_connection;

    enum class InterfaceState
    {
        NEW_CONNECTION,
        WAITING_FOR_USER,
        WAITING_FOR_PASS,
        LOGGED_IN
    } m_interfaceState;

    World::User *m_user;
    World::World &m_world;
    Grammar::Grammar &m_grammar;
    Dictionary::Tokenizer m_tokenizer;
};

}
}

#endif
