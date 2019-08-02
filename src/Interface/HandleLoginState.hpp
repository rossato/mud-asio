#ifndef HANDLE_LOGIN_STATE_HPP
#define HANDLE_LOGIN_STATE_HPP

#include "InterfaceStateBase.hpp"

namespace Mud
{
namespace Interface
{

class HandleLoginState : public InterfaceStateBase
{
public:
    HandleLoginState(MudInterface &interface)
        : InterfaceStateBase(interface) {}

    virtual void HandleLine() override;
    virtual void Prompt() override;
};

}
}

#endif
