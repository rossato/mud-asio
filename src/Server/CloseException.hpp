#ifndef CLOSE_EXCEPTION_HPP
#define CLOSE_EXCEPTION_HPP

#include <stdexcept>

namespace Mud
{
namespace Server
{

class CloseException : public std::exception {
    virtual const char *what() const noexcept override
        { return "Cleanly requesting close of connection"; }
};

}
}

#endif
