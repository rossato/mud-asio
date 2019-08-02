#ifndef MUD_PROGRAM_HPP
#define MUD_PROGRAM_HPP

#include "Actions/Actions.hpp"
#include "Grammar/Grammar.hpp"
#include "Http/HttpConnectionFactory.hpp"
#include "Server/Server.hpp"
#include "World/World.hpp"
#include "MudConnectionFactory.hpp"

namespace Mud
{
namespace Program
{

class MudProgram
{
public:
    MudProgram()
        : m_world(m_dictionary),
          m_mudFactory(m_grammar, m_server, m_world),
          m_httpFactory(m_server)
    {
        Actions::PopulateGrammarWithActions(m_grammar, m_dictionary);
    }

    void AcceptMudConnections(int port)
    {
        m_server.Accept(port, m_mudFactory);
    }

    void AcceptHttpConnections(int port)
    {
        m_server.Accept(port, m_httpFactory);
    }
    
    void Run() { m_server.Run(); }
    
    World::World &World() { return m_world; }
    Grammar::Grammar &Grammar() { return m_grammar; }
    Server::Server &Server() { return m_server; }
    
private:
    Dictionary::Dictionary m_dictionary;
    World::World m_world;
    Grammar::Grammar m_grammar;
    Server::Server m_server;
    MudConnectionFactory m_mudFactory;
    Http::HttpConnectionFactory m_httpFactory;
};

}
}

#endif
