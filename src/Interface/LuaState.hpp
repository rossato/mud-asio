#ifndef LUA_STATE_HPP
#define LUA_STATE_HPP

#include <sstream>
#include "InterfaceStateBase.hpp"

namespace Mud
{
namespace Interface
{

class LuaState : public InterfaceStateBase
{
public:
    LuaState(MudInterface &interface);

    virtual void HandleLine() override;
    virtual void Prompt() override;

private:
    std::ostringstream m_commandInProgress;
    bool m_multiline;
};

}
}

#endif
