#include <iostream>
#include "Logic/MudProgram.hpp"

using namespace Mud;

namespace
{
    const int TELNET_SERVER_PORT = 5000;
    const int   HTTP_SERVER_PORT = 5080;
}

int main()
{
    std::cout << "Program starting..." << std::endl;

    Logic::MudProgram program;
    program.AcceptMudConnections(TELNET_SERVER_PORT);
    program.AcceptHttpConnections(HTTP_SERVER_PORT);
    program.Run();
    
    std::cout << "Program terminating (normal shutdown)." << std::endl;
}
