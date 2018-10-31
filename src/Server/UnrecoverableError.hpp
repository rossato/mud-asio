#ifndef UNRECOVERABLE_ERROR_HPP
#define UNRECOVERABLE_ERROR_HPP

#include <string>

namespace Mud
{
namespace Server
{

class UnrecoverableError : public std::exception
{
public:
    UnrecoverableError(const std::string &message)
        : m_message(message) {}

    virtual const char *what() const noexcept override
    {
        return m_message.c_str();
    }

    const std::string &message() const { return m_message; }

private:
    const std::string m_message;
};

}
}

#endif
