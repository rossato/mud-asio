#include "TestConnection.hpp"
#include "HasWorld.hpp"

using namespace Mud::Test;

TestInterface::TestInterface()
    : Mud::Interface::MudInterface(theProgram(),
                                   m_io_service, output) {}
