#ifndef HANDLE_PASSWORD_STATE_HPP
#define HANDLE_PASSWORD_STATE_HPP

#include "InterfaceStateBase.hpp"

namespace Mud
{
namespace Interface
{

class HandlePasswordState : public InterfaceStateBase
{
public:
    HandlePasswordState(MudInterface &interface)
        : InterfaceStateBase(interface) {}

    virtual void HandleLine() override;
    virtual void Prompt() override;
};

}
}

#endif
