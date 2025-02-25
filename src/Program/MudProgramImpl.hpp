#ifndef MUD_PROGRAM_IMPL_HPP
#define MUD_PROGRAM_IMPL_HPP

#include "Actions/Actions.hpp"
#include "Http/HttpConnectionFactory.hpp"
#include "Parser/Parser.hpp"
#include "Server/Server.hpp"
#include "World/World.hpp"
#include "MudConnectionFactory.hpp"

namespace Mud
{
namespace Program
{

class MudProgram;
    
class MudProgramImpl
{
    friend MudProgram;
public:
    MudProgramImpl(MudProgram &program)
        : m_tokenizer(m_dictionary),
          m_world(m_dictionary),
          m_mudFactory(program),
          m_httpFactory(m_server)
    {
        Actions::PopulateParserWithActions(m_parser, m_dictionary);
    }
        
    void AcceptMudConnections(int port)
    {
        m_server.Accept(port, m_mudFactory);
    }

    void AcceptHttpConnections(int port)
    {
        m_server.Accept(port, m_httpFactory);
    }
private:
    Dictionary::Dictionary m_dictionary;
    Dictionary::Tokenizer m_tokenizer;
    World::World m_world;
    Interface::MudParser m_parser;
    Server::Server m_server;

    MudConnectionFactory m_mudFactory;
    Http::HttpConnectionFactory m_httpFactory;
};



}
}

#endif
