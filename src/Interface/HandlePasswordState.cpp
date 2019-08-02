#include "HandlePasswordState.hpp"

#include "Server/Ansi.hpp"
#include "HandleLoginState.hpp"
#include "MudInterface.hpp"
#include "ParsingGrammarState.hpp"

using namespace Mud::Interface;

void HandlePasswordState::HandleLine()
{
    m_interface.Write(ECHOON NEWLINE);
    auto &user = m_interface.User();
    
    if (user.Verify(m_interface.GetString()))
    {
        user.RegisterConnection(m_interface);
        std::cout << "New login for user " << user.Name() << "." NEWLINE;

        std::ostringstream out;
        out << "New login detected for user " << user.Name() << "." NEWLINE;
        user.Broadcast(out.str());

        m_interface << "Welcome, " << user.Name() << "!" NEWLINE;
        m_interface.ChangeState<ParsingGrammarState>();
    }
    else {
        m_interface.Write("Incorrect password" NEWLINE);
        m_interface.ChangeState<HandleLoginState>();
    }
}

void HandlePasswordState::Prompt()
{
    m_interface.Write("Password: " ECHOOFF);
}
