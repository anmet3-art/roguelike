#include "entity.h"

#include <cstdlib>

#include "game.h"

void Enemy::update(Game& game) {
    int playerX = game.getPlayer().getX();
    int playerY = game.getPlayer().getY();

    int selfX = getX(), selfY = getY();
    int newX = getX(), newY = getY();

    int diffX = playerX - selfX;
    int diffY = playerY - selfY;
    if (std::abs(diffX) + std::abs(diffY) == 1) {
        game.damagePlayer(getDamage());
        return;
    }
    if ((std::abs(diffX) + std::abs(diffY)) > visionRange) {
        return;
    }
    if (std::abs(diffX) > std::abs(diffY)) {
        if (diffX > 0) {
            newX += 1;
        } else {
            newX -= 1;
        }
    } else {
        if (diffY > 0) {
            newY += 1;
        } else {
            newY -= 1;
        }
    }
    if (game.isWalkable(newX, newY)) {
        Newcoord(newX, newY);
    }
}
void Enemy::interact(Game& game){
    takeDamage(game.getPlayer().getDamage());
    
}
