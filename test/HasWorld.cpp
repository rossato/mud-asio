#include "HasWorld.hpp"

namespace Mud
{
namespace Test
{

Logic::MudProgram &theProgram()
{
    static Logic::MudProgram program;
    return program;
}
    
}
}
