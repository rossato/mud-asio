#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

Program::MudProgram &theProgram()
{
    static Program::MudProgram program;
    return program;
}
    
}
}
