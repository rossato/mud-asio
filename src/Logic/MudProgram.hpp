#ifndef MUD_PROGRAM_HPP
#define MUD_PROGRAM_HPP

#include "Http/HttpConnectionFactory.hpp"
#include "Grammar/Grammar.hpp"
#include "Server/Server.hpp"
#include "World/World.hpp"
#include "Logic.hpp"
#include "MudConnectionFactory.hpp"

namespace Mud
{
namespace Logic
{

class MudProgram
{
public:
    MudProgram()
        : m_world(m_server, m_grammar, m_dictionary),
          m_mudFactory(m_world), m_httpFactory(m_server)
    {
        PopulateGrammarWithLogic(m_grammar, m_dictionary);
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

private:
    Dictionary::Dictionary m_dictionary;
    Grammar::Grammar m_grammar;
    Server::Server m_server;
    World::World m_world;
    MudConnectionFactory m_mudFactory;
    Http::HttpConnectionFactory m_httpFactory;
};

}
}

#endif
