#ifndef USAGE_EXCEPTION_HPP
#define USAGE_EXCEPTION_HPP

#include <string>

namespace Mud
{
namespace Grammar
{

class UsageException : public std::exception
{
public:
    UsageException(const std::string &message)
        : m_message(message) {}

    virtual const char *what() const noexcept override
    {
        return m_message.c_str();
    }

private:
    const std::string m_message;
};

}
}

#endif
