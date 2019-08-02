#include "Lua.hpp"

#include <lua.hpp>
#include "Dictionary/Dictionary.hpp"
#include "Server/Ansi.hpp"
#include "Noun.hpp"
#include "World.hpp"

#define WORLD_METATABLE "MudAsio.World"
#define NOUN_METATABLE "MudAsio.Noun"
#define CPP_REF "cppref"
#define PARENT "parent"

using namespace Mud::World;

namespace
{
int Print(lua_State *L)
{
    std::ostringstream &ostream = *static_cast<std::ostringstream*>(lua_touserdata(L, lua_upvalueindex(1)));
    for (int i = 1, top = lua_gettop(L); i <= top; ++i)
    {
        if (i > 1) ostream << " ";
        ostream << luaL_tolstring(L, i, nullptr);
    }
    ostream << NEWLINE;
    return 0;
}

struct WorldLua
{
    World *world;
};
World &CheckWorld(lua_State *L, int arg)
{
    return *(static_cast<struct WorldLua *>(luaL_checkudata(L, arg, WORLD_METATABLE))->world);
}

struct NounLua
{
    Noun *noun;
};    
Noun &CheckNoun(lua_State *L, int arg)
{
    return *(static_cast<struct NounLua *>(luaL_checkudata(L, arg, NOUN_METATABLE))->noun);
}

void PushNounToLua(lua_State *L, Noun &noun)
{
    // lua_rawgeti(L, LUA_REGISTRYINDEX, noun.LuaRef());
    // if (lua_isnil(L, -1))
    // {
    //     lua_pop(L, 1);
        auto &nounLua = *static_cast<struct NounLua *>(lua_newuserdata(L, sizeof(struct NounLua)));
        nounLua.noun = &noun;
        luaL_setmetatable(L, NOUN_METATABLE);
    //     lua_pushvalue(L, -1);
    //     noun.SetLuaRef(luaL_ref(L, LUA_REGISTRYINDEX));
    // }
}

int Create(lua_State *L)
{
    auto &world = CheckWorld(L, lua_upvalueindex(1));
    const auto name = luaL_checkstring(L, 1);
    int id = luaL_checkinteger(L, 2);
    luaL_checktype(L, 3, LUA_TTABLE);
    
    auto &location = world.CreateLocation(id, name,
                                          std::vector<Mud::Dictionary::Token>(
                                              {world.Dictionary().TryInsert(name, Mud::Dictionary::NOUN)}
                                              ));
    PushNounToLua(L, location);

    for (lua_pushnil(L); lua_next(L, 3);)
    {
        lua_pushvalue(L, -1);
        lua_copy(L, -3, -2);
        lua_settable(L, -4);
    }

    return 1;
}

int NounGetter(lua_State *L)
{
    auto &noun = CheckNoun(L, 1);
    const auto field = lua_tostring(L, 2);
    if (!strcmp("description", field))
    {
        lua_pushlstring(L, noun.Description().c_str(), noun.Description().size());
    }
    else if (!strcmp("parent", field))
    {
        auto parent = noun.GetLocation();
        if (parent)
        {
            PushNounToLua(L, *parent);
        }
        else
        {
            lua_pushnil(L);
        }
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

int NounSetter(lua_State *L)
{
    auto &world = CheckWorld(L, lua_upvalueindex(1));
    auto &noun = CheckNoun(L, 1);
    const auto field = lua_tostring(L, 2);
    if (!strcmp("description", field))
    {
        noun.WithDescription(luaL_checkstring(L, 3));
    }
    else if (!strcmp("exits", field))
    {
        luaL_checktype(L, 3, LUA_TTABLE);
        for (lua_pushnil(L); lua_next(L, 3); lua_pop(L, 1))
        {
            try
            {
                auto token = world.Dictionary().LookUp(luaL_checkstring(L, -2));
                auto dir = TokenToDirection(token);
                luaL_argcheck(L, dir, 4, "direction name is not a direction");
                int neighbor = luaL_checkinteger(L, -1);
                noun.SetExit(dir, neighbor);
            }
            catch(const Mud::Dictionary::WordNotFoundException &)
            {
                luaL_argerror(L, 4, "direction name not recognized");
            }
        }
    }
    else if (!strcmp("parent", field))
    {
        Noun *parent = nullptr;
        if (!lua_isnil(L, 3))
        {
            parent = &CheckNoun(L, 3);
        }

        auto oldparent = noun.GetLocation();
        if (oldparent)
        {
            oldparent->RemoveItem(noun);
        }

        if (parent)
        {
            parent->AddItem(noun);
            noun.SetLocation(parent);
        }
    }
    return 0;
}

int Get(lua_State *L)
{
    auto &world = CheckWorld(L, 1);
    try
    {
        PushNounToLua(L, world.GetLocation(luaL_checkinteger(L, 2)));
    }
    catch(const std::out_of_range &)
    {
        luaL_argerror(L, 2, "no location at that index");
    }

    return 1;
}

int Reload(lua_State *L)
{
    auto &world = CheckWorld(L, lua_upvalueindex(1));
    if (luaL_dofile(L, "data/world.lua"))
    {
        std::cerr << "Error loading script: " << lua_tostring(L, -1) << std::endl;
    }
    world.FinalizeLocations();
    return 0;
}

const struct luaL_Reg world_methods[] =
{
    { "Create", Create },
    { "Get", Get },
    { "Reload", Reload },
    { nullptr, nullptr }
};

}

Lua::Lua(World &world)
    : L(luaL_newstate())
{
    std::cerr << "Initializing MUD world with Lua..." << std::endl;

    // luaL_openlibs(L); // io and os have potential security issues for remote users
    luaL_requiref(L, "_G", luaopen_base, 1);
    luaL_requiref(L, "table", luaopen_table, 1);
    luaL_requiref(L, "string", luaopen_string, 1);
    
    auto &worldLua = *static_cast<struct WorldLua *>(lua_newuserdata(L, sizeof(struct WorldLua)));
    worldLua.world = &world;

    luaL_newmetatable(L, WORLD_METATABLE);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pushvalue(L, -2);
    luaL_setfuncs(L, world_methods, 1);

    lua_setmetatable(L, -2);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "World");

    luaL_newmetatable(L, NOUN_METATABLE);
    lua_pushcfunction(L, NounGetter);
    lua_setfield(L, -2, "__index");

    lua_pushvalue(L, -2);
    lua_pushcclosure(L, NounSetter, 1);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);

    lua_getfield(L, -1, "Reload");
    lua_call(L, 0, 0);
    lua_pop(L, 1);
    
    lua_getglobal(L, "print");
    lua_setglobal(L, "log");

    lua_pushlightuserdata(L, &m_printBuffer);
    lua_pushcclosure(L, Print, 1);
    lua_setglobal(L, "print");

    // todo hide dofile and loadfile from interactive use
    
    lua_settop(L, 0);
}

Lua::~Lua()
{
    lua_close(L);
}

// Returns true if Lua thinks it's a complete statement (even if it has errors)
bool Lua::HandleLine(const std::string &line, std::ostream &response)
{
    m_printBuffer.str("");

    std::string expression("return ");
    expression += line + ';';

    if (luaL_dostring(L, expression.c_str()))
    {
        lua_pop(L, 1);
        if (luaL_dostring(L, line.c_str()))
        {
            std::string error = lua_tostring(L, -1);
            lua_settop(L, 0);
            if (error.substr(error.size()-5) == "<eof>")
            {
                return false;
            }
            else
            {
                response << "Error: " << error << NEWLINE;
                return true;
            }
        }
    }

    response << m_printBuffer.str();

    for (int i = 1, top = lua_gettop(L); i <= top; ++i)
    {
        response << luaL_tolstring(L, i, nullptr) << NEWLINE;
    }
    lua_settop(L, 0);
    return true;
}
