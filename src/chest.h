#pragma once
#include "entity.h"
#include "item.h"
#include <vector>
#include <memory>
class Chest: public Entity{
private:
    std::vector<std::unique_ptr<Item>> items;
public:
    Chest(int x, int y) : Entity(x, y, 'C', 1, 0){
        items.push_back(std::make_unique<HealPotion>(5));
    }
    bool isDead() const override { return items.empty(); } ;
    void interact(Game& game) override;
};
