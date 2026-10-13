#include "item.h"

#include "game.h"

void HealPotion::use(Game& game) {
    game.healPlayer(healpoint);
}
