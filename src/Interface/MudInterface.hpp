#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include "Dictionary/Tokenizer.hpp"
#include "Server/ConnectionBase.hpp"
#include "World/World.hpp"
#include "InterfaceStateBase.hpp"

namespace Mud
{
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
class User;
}

namespace Interface
{

class MudInterface : public Server::ConnectionBase, public Dictionary::Tokenizer {
public:
    template <class... Args>
    MudInterface(Grammar::Grammar &grammar, Server::Server &server, World::World &world,
                 Args &&... args)
        : Server::ConnectionBase(std::forward<Args>(args)...),
          Dictionary::Tokenizer(world.Dictionary()),
          m_grammar(grammar),
          m_server(server),
          m_world(world)
    {
        Welcome();
    }

    void HandleLine(const std::string &line)
    {
        Str(line);
        m_state->HandleLine();
        m_state->Prompt();
    }
    void HandleClose();

    void SetUser(World::User &user) { m_user = &user; }
    World::User &User() const { return *m_user; }

    Grammar::Grammar &Grammar() const { return m_grammar; }
    Server::Server   &Server()  const { return m_server; }
    World::World     &World()   const { return m_world; }
    
    template <class StateType>
    void ChangeState()
    {
        m_state = std::make_unique<StateType>(*this);
    }
    
private:
    void Welcome();

    World::User *m_user;
    Grammar::Grammar &m_grammar;
    Server::Server &m_server;
    World::World &m_world;
    
    std::unique_ptr<InterfaceStateBase> m_state;
};

}
}

#endif
