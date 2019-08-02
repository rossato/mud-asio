#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include "Direction.hpp"

namespace Mud
{
namespace World
{
class User;

class Location
{
//    friend class WorldBuilder;
    friend class World;
public:
    // template <class... Args>
    // Noun &EmplaceItem(Args &&...args)
    // {
    //     return m_itemRepops.emplace_back(std::forward<Args>(args)...);
    // }

//    void Repop();    

    Location &TellAllBut(User &actor, const std::string &message);
    
    const std::vector<User*> &GetUsers() const { return m_usersHere; }
    void UserArriving(User &);
    void UserLeaving(User &, Direction);    

private:
    std::vector<User*> m_usersHere;
//    std::vector<Noun> m_itemRepops;
};

}
}

#endif
