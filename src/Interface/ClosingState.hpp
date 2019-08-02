#ifndef CLOSING_STATE_HPP
#define CLOSING_STATE_HPP

#include "InterfaceStateBase.hpp"

namespace Mud
{
namespace Interface
{

class ClosingState : public InterfaceStateBase
{
public:
    ClosingState(MudInterface &interface)
        : InterfaceStateBase(interface) {}

    virtual void HandleLine() override {}
    virtual void Prompt() override {}
};

}
}

#endif
