#include "MudInterface.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "Grammar/Grammar.hpp"
#include "Server/Ansi.hpp"
#include "Server/ConnectionBase.hpp"
#include "Server/Server.hpp"
#include "World/User.hpp"
#include "World/World.hpp"
#include "BasicActions.hpp"

using namespace Mud::Logic;

MudInterface::MudInterface(Server::ConnectionBase &connection,
                           World::World &world)
    : m_connection(connection),
      m_interfaceState(InterfaceState::WAITING_FOR_USER),
      m_world(world),
      m_grammar(world.Grammar()),
      m_tokenizer(world.Dictionary())
{
    m_connection.Write("Welcome!" NEWLINE "Who are you: ");
}

MudInterface::~MudInterface()
{
    if (m_user) m_user->DeregisterConnection(m_connection);
}

void MudInterface::HandleLoginPrompt()
{
    m_user = m_world.GetUser(m_tokenizer.GetString());
    if (m_user) {
        m_connection.Write("Password: " ECHOOFF);
        m_interfaceState = InterfaceState::WAITING_FOR_PASS;
    }
    else {
        m_connection.Write("User not found" NEWLINE "Who are you: ");
    }
}

void MudInterface::HandlePassPrompt()
{
    m_connection.Write(ECHOON NEWLINE);

    if (m_user->Verify(m_tokenizer.GetString()))
    {
        m_user->RegisterConnection(m_connection);
        std::cout << "New login for user " << m_user->Name() << "." NEWLINE;

        std::ostringstream out;
        out << "New login detected for user " << m_user->Name() << "." NEWLINE;
        m_user->Broadcast(out.str());

        m_connection << "Welcome, " << m_user->Name() << "!" NEWLINE NEWLINE;
        m_interfaceState = InterfaceState::LOGGED_IN;

        LookAction::Act(*m_user, m_connection.ostream(), 0, 0);
        DisplayPrompt();
    }
    else {
        m_connection.Write("Incorrect password" NEWLINE "Who are you: ");
        m_interfaceState = InterfaceState::WAITING_FOR_USER;
    }
}

void MudInterface::DisplayPrompt()
{
    m_connection.Write(NEWLINE "> ");
}

void MudInterface::HandleOrdinaryLine()
{
    m_grammar.Parse(*m_user, m_connection.ostream(), m_tokenizer);
    DisplayPrompt();
}

void MudInterface::HandleLine(const std::string &line)
{
    m_tokenizer.Str(line);

    switch (m_interfaceState)
    {
    case InterfaceState::WAITING_FOR_USER:
        HandleLoginPrompt();
        break;
    case InterfaceState::WAITING_FOR_PASS:
        HandlePassPrompt();
        break;
    case InterfaceState::LOGGED_IN:
        HandleOrdinaryLine();
        break;
    default:
        throw std::logic_error("Unknown interface state.");
    }
}
