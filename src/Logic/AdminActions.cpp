#include "AdminActions.hpp"

using namespace Mud::Logic;

const std::string ShutdownAction::Description("Shut down the server at a specified time (in seconds from now)");
const std::string ShutdownNowAction::Description("Shut down the server now");
const std::string ShutdownHelpAction::Description("Explain shutdown options");
const std::string ShutdownCancelAction::Description("Cancel a previously scheduled shutdown");
