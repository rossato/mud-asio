#ifndef USER_READER
#define USER_READER

#include <sstream>
#include "World.hpp"
#include "WorldBuilder.hpp"

namespace Mud
{
namespace World
{

class UserReader;

class IUserReaderState
{
public:
    explicit IUserReaderState(UserReader &reader)
        : m_reader(reader) {}

    virtual ~IUserReaderState() {}

    virtual void HandleLine(std::istream &lineStream) = 0;

protected:
    UserReader &m_reader;
};

class NewUserState : public IUserReaderState
{
public:
    explicit NewUserState(UserReader &reader)
        : IUserReaderState(reader) {}

    virtual void HandleLine(std::istream &lineStream) override;
};
    
class UserReader
{
public:
    explicit UserReader(World &world)
        : m_world(world),
          m_builder(world),
          m_currentUser(nullptr),
          m_state(std::make_unique<NewUserState>(*this))
    {}

    void Read(std::istream &file);

    WorldBuilder &Builder() { return m_builder; }

    template <class... Args>
    void NewUser(Args &&... args)
    {
        m_currentUser = &m_builder.NewUser(std::forward<Args>(args)...);
    }

    User *CurrentUser() const { return m_currentUser; }
    
private:
    World &m_world;
    WorldBuilder m_builder;
    User *m_currentUser;
    std::unique_ptr<IUserReaderState> m_state;
};

}
}

#endif
