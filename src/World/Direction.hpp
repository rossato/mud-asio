#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <iostream>
#include "Dictionary/Token.hpp"

namespace Mud
{
namespace World
{
enum Direction
{
    NODIR = 0,
    N,
    E,
    S,
    W,
    NE,
    SE,
    SW,
    NW,
    U,
    D,
    IN,
    OUT
};

Direction TokenToDirection(Dictionary::Token token);
    
std::ostream &operator<<(std::ostream &, Direction);

}
}


#endif
