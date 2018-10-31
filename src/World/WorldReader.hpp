#ifndef WORLD_READER
#define WORLD_READER

#include <sstream>
#include "World.hpp"
#include "WorldBuilder.hpp"

namespace Mud
{
namespace Dictionary
{
class Tokenizer;
}

namespace World
{

class WorldReader;

class IWorldReaderState
{
public:
    explicit IWorldReaderState(WorldReader &reader)
        : m_reader(reader) {}

    virtual ~IWorldReaderState() {}

    virtual void HandleLine(std::istream &lineStream) = 0;

protected:
    WorldReader &m_reader;
};

class NewRoomState : public IWorldReaderState
{
public:
    explicit NewRoomState(WorldReader &reader)
        : IWorldReaderState(reader) {}

    virtual void HandleLine(std::istream &lineStream) override;
};
    
class WorldReader
{
public:
    explicit WorldReader(World &world)
        : m_world(world),
          m_builder(world),
          m_state(std::make_unique<NewRoomState>(*this))
    {}

    ~WorldReader()
    {
        m_builder.NewUser("Ken", 1000);
        m_builder.NewUser("Paul", 1001);
    }

    void Read(std::istream &file);

    WorldBuilder &Builder() { return m_builder; }
    
    template <class... Args>
    void NewLocation(Args &&... args)
    {
        m_currentLocation = &m_builder.NewLocation(std::forward<Args>(args)...);
    }
    Location &CurrentLocation() { return *m_currentLocation; }

    template <class StateType>
    void ChangeState()
    {
        m_state = std::make_unique<StateType>(*this);
    }
private:
    World &m_world;
    WorldBuilder m_builder;
    Location *m_currentLocation;
    std::unique_ptr<IWorldReaderState> m_state;
};

}
}

#endif
