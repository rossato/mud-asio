#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include "Dictionary/Tokenizer.hpp"
#include "Program/MudProgram.hpp"
#include "Server/ConnectionBase.hpp"
#include "InterfaceStateBase.hpp"
#include "MudParser.hpp"

namespace Mud
{
namespace Grammar
{
template <class ContextType> class Parser;
}

namespace Server
{
class Server;
}

namespace World
{
class User;
class World;
}

namespace Interface
{

class MudInterface : public Server::ConnectionBase {
public:
    template <class... Args>
    MudInterface(Program::MudProgram &program,
                 Args &&... args)
        : Server::ConnectionBase(std::forward<Args>(args)...),
          m_tokenizer(program.Dictionary()),
          m_program(program)
    {
        Welcome();
    }

    template <class... Args>
    MudInterface &operator=(Args &&... args)
    {
        Server::ConnectionBase::operator=(std::forward<Args>(args)...);

        Welcome();

        return *this;
    }

    void HandleLine(const std::string &line)
    {
        m_tokenizer.Str(line);
        m_state->HandleLine();
        m_state->Prompt();
    }
    void HandleClose();

    void SetUser(World::User &user) { m_user = &user; }
    World::User &User() const { return *m_user; }

    Dictionary::Tokenizer &Tokenizer()       { return m_tokenizer; }
    MudParser             &Parser()    const { return m_program.Parser(); }
    Server::Server        &Server()    const { return m_program.Server(); }
    World::World          &World()     const { return m_program.World();  }
    
    template <class StateType>
    void ChangeState()
    {
        m_state = std::make_unique<StateType>(*this);
    }
    
private:
    void Welcome();

    World::User *m_user;
    Dictionary::Tokenizer m_tokenizer;
    Program::MudProgram &m_program;
    
    std::unique_ptr<InterfaceStateBase> m_state;
};

}
}

#endif
