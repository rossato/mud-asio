#include "MudInterface.hpp"

#include "Server/Ansi.hpp"
#include "World/User.hpp"
#include "HandleLoginState.hpp"

using namespace Mud::Interface;

void MudInterface::Welcome()
{
    Write("Welcome!" NEWLINE);
    ChangeState<HandleLoginState>();
    m_state->Prompt();
}

void MudInterface::HandleClose()
{
    if (m_user) m_user->DeregisterConnection(*this);
}
