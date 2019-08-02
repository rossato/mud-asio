#ifndef WORLD_BUILDER
#define WORLD_BUILDER

#include <string>
#include <vector>

namespace Mud
{
namespace World
{
class Noun;
class User;
class World;

class WorldBuilder
{
public:
    WorldBuilder(World &world)
        : m_world(world) {}

    Noun     &NewLocation(int index, const std::string &name);
    Noun     &NewItem(const std::vector<std::string> &words);
    User     &NewUser(const std::string &name, int location);

private:
    World &m_world;
    Noun *m_lastLocation;
};

}
}

#endif
