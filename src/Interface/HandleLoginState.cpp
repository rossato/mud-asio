#include "HandleLoginState.hpp"

#include "Server/Ansi.hpp"
#include "HandlePasswordState.hpp"
#include "MudInterface.hpp"

using namespace Mud::Interface;

void HandleLoginState::HandleLine()
{
    auto userName = m_interface.GetString();
    if (!World::User::IsSafeUserName(userName))
    {
        m_interface.Write("User names may only consist of letters and must be at least 1 letter long." NEWLINE);
        return;
    }
    auto user = m_interface.World().GetUser(userName);
    if (user)
    {
        m_interface.SetUser(*user);
        m_interface.ChangeState<HandlePasswordState>();
    }
    else {
        m_interface.Write("User not found." NEWLINE);
    }
}

void HandleLoginState::Prompt()
{
    m_interface.Write("Who are you: ");
}
