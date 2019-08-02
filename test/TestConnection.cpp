#include "TestConnection.hpp"
#include "HasWorld.hpp"

using namespace Mud::Test;

TestInterface::TestInterface()
    : Mud::Interface::MudInterface(theProgram().Grammar(),
                                   theProgram().Server(),
                                   theProgram().World(),
                                   m_io_service, output) {}
