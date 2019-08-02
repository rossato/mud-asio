#ifndef MUD_LUA_HPP
#define MUD_LUA_HPP

#include <sstream>
#include <string>

struct lua_State;

namespace Mud
{
namespace World
{
class World;

class Lua
{
public:
    Lua(World &world);
    ~Lua();

    bool HandleLine(const std::string &line, std::ostream &response);

private:
    lua_State *L;
    std::ostringstream m_printBuffer;
};

}
}

#endif
