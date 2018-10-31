#ifndef WORLD_TEST_HPP
#define WORLD_TEST_HPP

#include "Dictionary/Tokenizer.hpp"
#include "Logic/MudProgram.hpp"
#include "HeadlessUser.hpp"

namespace Mud
{
namespace Test
{

Logic::MudProgram &theProgram();
    
class ResetsWorld
{
protected:
    ResetsWorld()
        : world(theProgram().World())
        { world.TestInit(); }

    World::World &world;
};

class WorldTest : public ResetsWorld
{
protected:
    WorldTest()
        : ken(world.GetUser("ken"))
        {}

    HeadlessUser ken;
};
    
}
}

#endif
