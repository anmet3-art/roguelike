#pragma once
#include "entity.h"
#include "item.h"
#include <vector>
#include <memory>
class Chest: public Entity{
private:

    std::vector<std::unique_ptr<Item>> items;
public:
    Chest(int x, int y, char symbol, int hp, int damage) : Entity(x, y, 'C', hp, damage){
        auto items_charact = std::make_unique<HealPotion>(5);
        items.push_back(std::move(items_charact));
    }
    bool isDead() const { return items.empty(); } override;
    void interact(Game& game) override;
};
