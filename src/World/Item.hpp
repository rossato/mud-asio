#ifndef ITEM_HPP
#define ITEM_HPP

#include "Noun.hpp"

namespace Mud
{
namespace World
{

class Item : public Noun
{
public:
    Item(const std::string &name,
         std::vector<Dictionary::Token> &&referents)
        : Noun(name, std::move(referents)) {}
    
    template <class T>
    Item &SetDescription(T&& description)
    {
        m_description = std::forward<T>(description);
        return *this;
    }

private:
    std::string m_description;
};

class ItemReference
{
public:
    enum ItemLocation
    {
        INVENTORY,
        HERE
    };

    ItemReference(Item *item_, ItemLocation location_)
        : item(item_), location(location_) {}

    explicit operator bool() const { return item != nullptr; }
    
    Item *item;
    ItemLocation location;
};

}
}

std::ostream &operator<<(std::ostream &, const Mud::World::ItemReference &);

#endif
