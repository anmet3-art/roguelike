#pragma once
#include <memory>
#include <vector>

#include "constants.h"
#include "entity.h"
#include "item.h"
class Chest : public Entity {
   private:
    std::vector<std::unique_ptr<Item>> items;

   public:
    Chest(int x, int y, std::unique_ptr<Item> item) : Entity(x, y, consts::CHEST_SYMBOL, consts::CHEST_HP, consts::CHEST_DAMAGE) {
        items.push_back(std::move(item));
    }
    bool isDead() const override { return items.empty(); }
    void interact(Game& game) override;
};
