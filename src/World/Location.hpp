#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Direction.hpp"
#include "Item.hpp"
#include "Noun.hpp"
#include "User.hpp"

namespace Mud
{
namespace World
{
class Location : public Noun
{
    friend std::ostream &operator<<(std::ostream &, const Location &);
    friend class WorldBuilder;
public:
    explicit Location(const std::string &name,
                      std::vector<Dictionary::Token> &&referents)
        : Noun(name, std::move(referents)) {}

    // Observers are out there
    Location(const Location &)            = delete;
    Location &operator=(const Location &) = delete;
    
    template <class T>
    Location &SetDescription(T&& description)
    {
        m_description = std::forward<T>(description);
        return *this;
    }

    Location &SetExit(Direction dir, int otherLocation)
    {
        m_exitIndexes.emplace_back(dir, otherLocation);
        return *this;
    }

    template <class... Args>
    Noun &EmplaceItem(Args &&...args)
    {
        return m_itemRepops.emplace_back(std::forward<Args>(args)...);
    }

    void Repop()
    {
        m_itemsHere.clear();
        for (auto &item : m_itemRepops)
        {
            m_itemsHere.emplace_back(&item);
        }
    }
    
    Location *GetExit(Direction dir) const
    {
        auto it = m_exits.find(dir);
        if (it == m_exits.end()) return nullptr;
        return it->second;
    }

    template <class T>
    Location &TellAllBut(User &actor, const T &message)
    {
        for (auto user : m_usersHere)
        {
            if (user != &actor) user->Broadcast(message);
        }
        return *this;
    }
    
    const std::vector<User*> &GetUsers() const { return m_usersHere; }
    void UserArriving(User &);
    void UserLeaving(User &, Direction);

    const std::vector<Noun*> &GetItems() const { return m_itemsHere; }
    void RemoveItem(Noun &);
    void AddItem(Noun &noun)
        { m_itemsHere.emplace_back(&noun); }

private:
    std::string m_description;

    std::vector<std::pair<Direction, int> > m_exitIndexes;
    std::map<Direction, Location*> m_exits;

    std::vector<User*> m_usersHere;
    std::vector<Noun> m_itemRepops;
    std::vector<Noun*> m_itemsHere;
};

std::ostream &operator<<(std::ostream &stream, const Mud::World::Location &location);

}
}

#endif
