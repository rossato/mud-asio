#ifndef INTERFACE_STATE_BASE_HPP
#define INTERFACE_STATE_BASE_HPP

namespace Mud
{
namespace Dictionary
{
class Tokenizer;
}

namespace Interface
{
class MudInterface;
    
class InterfaceStateBase
{
protected:
    InterfaceStateBase(MudInterface &interface)
        : m_interface(interface) {}

public:
    virtual ~InterfaceStateBase() {}
    virtual void HandleLine(Dictionary::Tokenizer &) = 0;
    virtual void Prompt() = 0;
    
protected:
    MudInterface &m_interface;
};
    
}
}

#endif
