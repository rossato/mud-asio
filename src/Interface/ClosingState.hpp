#ifndef CLOSING_STATE_HPP
#define CLOSING_STATE_HPP

#include "InterfaceStateBase.hpp"

namespace Mud
{
namespace Interface
{

// This isn't that useful, but it at least suppresses prompts after quitting
class ClosingState : public InterfaceStateBase
{
public:
    ClosingState(MudInterface &interface)
        : InterfaceStateBase(interface) {}

    virtual void HandleLine(Dictionary::Tokenizer &) override {}
    virtual void Prompt() override {}
};

}
}

#endif
