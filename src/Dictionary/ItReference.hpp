#ifndef IT_REFERENCE_HPP
#define IT_REFERENCE_HPP

namespace Mud
{
namespace World
{
class Noun;
}

namespace Dictionary
{

class ItReference
{
public:
    World::Noun *It() const { return m_it; }
    void SetIt(World::Noun *it) { m_it = it; }
    
private:
    World::Noun *m_it;
};

}
}

#endif
