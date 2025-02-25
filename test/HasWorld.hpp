#ifndef WORLD_TEST_HPP
#define WORLD_TEST_HPP

#include "Program/MudProgram.hpp"
#include "World/World.hpp"
#include "TestConnection.hpp"
//#include "HeadlessUser.hpp"

namespace Mud
{
namespace Test
{

Program::MudProgram &theProgram();
    
class ResetsWorld
{
protected:
    ResetsWorld()
    {
        theProgram().World().TestInit();
    }
};

class HasWorld : public ResetsWorld
{
protected:
    HasWorld()
        : world(theProgram().World()),
          tokenizer(theProgram().Tokenizer()),
          ken("ken")
        {}

    World::World &world;
    Dictionary::Tokenizer &tokenizer;
    TestInterface ken;
};
    
}
}

#endif
