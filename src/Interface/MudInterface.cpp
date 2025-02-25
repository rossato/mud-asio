#include "MudInterface.hpp"

#include "Dictionary/Tokenizer.hpp"
#include "Server/Ansi.hpp"
#include "World/User.hpp"
#include "World/World.hpp"
#include "HandleLoginState.hpp"

using namespace Mud::Interface;

void MudInterface::Welcome()
{
    Write("Welcome!" NEWLINE);
    ChangeState<HandleLoginState>();
    m_state->Prompt();
}

void MudInterface::HandleLine(const std::string &line)
{
    m_tokenizer.Str(line);
    m_state->HandleLine(m_tokenizer);
    m_state->Prompt();
}

void MudInterface::HandleClose()
{
    if (m_user) m_user->DeregisterConnection(*this);
}

Mud::World::User *MudInterface::LoadUser(const std::string &userName)
{
    return m_user = m_program.World().GetUser(userName);
}
