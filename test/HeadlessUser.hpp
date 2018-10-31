#ifndef HEADLESS_USER_HPP
#define HEADLESS_USER_HPP

#include "Dictionary/Tokenizer.hpp"
#include "Grammar/Grammar.hpp"
#include "World/User.hpp"
#include "World/World.hpp"
#include "TestConnection.hpp"

namespace Mud
{
namespace Test
{

class HeadlessUser
{
public:
    HeadlessUser(Mud::World::User *user_)
        : user(*user_), tokenizer(user_->World().Dictionary())
    {
        user.RegisterConnection(connection);
        connection.output.str("");
    }

    ~HeadlessUser()
    {
        user.DeregisterConnection(connection);
    }
    
    void RunCommand(const std::string &line)
    {
        tokenizer.Str(line);
        user.World().Grammar().Parse(user, connection.output, tokenizer);
    }

    operator Mud::World::User&() { return user; }

    TestConnection connection;
    Mud::World::User &user;
    Mud::Dictionary::Tokenizer tokenizer;
};

}
}

#endif
