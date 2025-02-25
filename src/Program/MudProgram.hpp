#ifndef MUD_PROGRAM_HPP
#define MUD_PROGRAM_HPP

#include <memory>
#include "Interface/MudParser.hpp"

namespace Mud
{
namespace Dictionary
{
class Dictionary;
class Tokenizer;
}
namespace Server
{
class Server;
}
namespace World
{
class World;
}

namespace Program
{

// This class holds onto 4 global objects that may need to be accessed.
// Let's use PIMPL to hide the complete types since not everyone needs all type definitions.

// This does have the downside of adding a layer of indirection (references instead of actual object locations)
//  But this is irrelevant if consumers hold the reference themselves regardless.
//  It matters only if consumers come all the way back here via inlines to get references.

class MudProgramImpl;
    
class MudProgram
{
public:
    MudProgram();
    ~MudProgram();

    void AcceptMudConnections(int port);
    void AcceptHttpConnections(int port);
    
    void Run();

    Dictionary::Dictionary &Dictionary() { return m_dictionary; }
    Dictionary::Tokenizer  &Tokenizer()  { return m_tokenizer;  }
    World::World           &World()      { return m_world;      }
    Interface::MudParser   &Parser()     { return m_parser;     }
    Server::Server         &Server()     { return m_server;     }
    
private:
    std::unique_ptr<MudProgramImpl> m_impl;

    Dictionary::Dictionary &m_dictionary;
    Dictionary::Tokenizer &m_tokenizer;
    World::World &m_world;
    Interface::MudParser &m_parser;
    Server::Server &m_server;
};

}
}

#endif
