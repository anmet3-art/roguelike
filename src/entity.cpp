#include "entity.h"

#include <cstdlib>

#include "game.h"

void Enemy::update(Game& game) {
    int playerX = game.getPlayer().getX();
    int playerY = game.getPlayer().getY();

    int selfX = getX(), selfY = getY();

    int diffX = playerX - selfX;
    int diffY = playerY - selfY;
    if (std::abs(diffX) + std::abs(diffY) == 1) {
        game.damagePlayer(getDamage());
        return;
    }
    if ((std::abs(diffX) + std::abs(diffY)) > visionRange && !enemyWary) {
        return;
    }
    enemyWary=true;
    Point p = game.nextStepToPlayer(selfX, selfY);
    if (p.x != consts::NO_CELL && p.y != consts::NO_CELL) {
        setPosition(p.x, p.y);
    }
}
void Enemy::interact(Game& game) { takeDamage(game.getPlayer().getDamage()); }
void Player::pushItem(std::unique_ptr<Item> item) { playerInventory.push_back(std::move(item)); }
bool Player::hasItem() const { return (!playerInventory.empty()); }
std::unique_ptr<Item> Player::takeItem() {
    std::unique_ptr<Item> itemAktiv;
    if (hasItem()) {
        itemAktiv = std::move(playerInventory.back());
        playerInventory.pop_back();
    }
    return itemAktiv;
}
int Player::getItemCount() const { return static_cast<int>(playerInventory.size()); }
std::string Player::getItemNames() const {
    std::string result;
    for (const auto& item : playerInventory) {
        if (!result.empty()) {
            result += ", ";
        }
        result += item->getName();
    }
    return result;
}
bool Player::hasSpace() const { return (getItemCount() < consts::INVENTORY_MAX_ITEMS); }
