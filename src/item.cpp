#include "item.h"

#include "game.h"

void HealPotion::use(Game& game) {
    game.healPlayer(healpoint);
}
std::string HealPotion::getName() const{
    return ("Heal+"+std::to_string(healpoint));
}


void PowerPotion::use(Game& game) {
    game.boostPlayerDamage(damageIncrease);
}
std::string PowerPotion::getName() const{
    return ("Power+"+std::to_string(damageIncrease));
}
