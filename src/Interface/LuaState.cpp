#include "LuaState.hpp"

#include "Server/Ansi.hpp"
#include "MudInterface.hpp"
#include "ParsingGrammarState.hpp"

using namespace Mud::Interface;

LuaState::LuaState(MudInterface &interface)
    : InterfaceStateBase(interface), m_multiline(false)
{
    std::cerr << "ALERT Debug console entered by user " << interface.User().Name() << "." << std::endl;;
}

void LuaState::HandleLine()
{
    auto line = m_interface.DumpRestOfLine();
    if (line.empty())
    {
        m_commandInProgress.str("");
        m_multiline = false;
        return;
    }
    else if (line == "quit")
    {
        m_interface.Write("Leaving debug mode..." NEWLINE);
        m_interface.ChangeState<ParsingGrammarState>();
        return;
    }

    auto &ostream = m_interface.ostream();
    auto &lua = m_interface.World().Lua();

    m_commandInProgress << line << '\n';
    if (lua.HandleLine(m_commandInProgress.str(), ostream))
    {
        m_commandInProgress.str("");
        m_multiline = false;
    }
    else
    {
        m_multiline = true;
    }
}

void LuaState::Prompt()
{
    if (m_multiline)
    {
        m_interface.Write("lua~# ");
    }
    else
    {
        m_interface.Write("lua# ");
    }
}
