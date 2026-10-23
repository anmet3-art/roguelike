#include "chest.h"

#include "game.h"
void Chest::interact(Game& game){
    for(auto& x: items){
        game.getPlayer().pushItem(std::move(x));
    }
    items.clear();


}
