#include "AdminActions.hpp"
#include "Interface/LuaState.hpp"

using namespace Mud::Actions;

const std::string ShutdownAction::Description("Shut down the server at a specified time (in seconds from now)");
const std::string ShutdownNowAction::Description("Shut down the server now");
const std::string ShutdownHelpAction::Description("Explain shutdown options");
const std::string ShutdownCancelAction::Description("Cancel a previously scheduled shutdown");
const std::string DebugAction::Description("Enter Lua debugging mode");

void DebugAction::Act(InterfaceType &interface, int, int)
{
    interface.Write(
        NEWLINE
        "Entering Lua debug console." NEWLINE
        "Type \"quit\" to return to grammar mode." NEWLINE
        "Type an empty line to abort a multi-line command (indicated with \"~\")." NEWLINE
        );
    interface.ChangeState<Interface::LuaState>();
}
