#include "Direction.hpp"

#include <vector>
#include "Dictionary/Dictionary.hpp"

namespace Mud
{
namespace World
{

namespace
{
    const std::vector<std::string> DirectionStrings {
        "somewhere",
        "north",
        "east",
        "south",
        "west",
        "northeast",
        "southeast",
        "southwest",
        "northwest",
        "up",
        "down",
        "in",
        "out"
    };
}

Direction TokenToDirection(Dictionary::Token token)
{
    if (!token.IsDirection()) return World::NODIR;

    int direction = (token.GetRawIndex() -
                     Dictionary::Dictionary::DirectionOffset) / 2;
    if (direction == 10 && token.index % 2) direction = 11; // out
    ++direction; // 1-indexed, 0 is nodir
    return World::Direction(direction);
}
    
std::ostream &operator<<(std::ostream &stream, Direction dir)
{
    if (dir == NODIR)
        return stream << DirectionStrings[0];

    return stream << "the " << DirectionStrings[dir];
}

}
}
