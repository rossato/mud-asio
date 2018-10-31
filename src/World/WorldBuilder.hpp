#ifndef WORLD_BUILDER
#define WORLD_BUILDER

#include "Dictionary/Dictionary.hpp"
#include "Location.hpp"

namespace Mud
{
namespace World
{

class WorldBuilder
{
public:
    WorldBuilder(World &world)
        : m_world(world) {}
    ~WorldBuilder();
    
    Location &NewLocation(int index, const std::string &name);
    Noun     &NewItem(const std::vector<std::string> &words);
    User     &NewUser(const std::string &name, int location);
    
private:
    World &m_world;
    Location *m_lastLocation;
};

}
}

#endif
