#include "chest.h"

#include "game.h"
void Chest::interact(Game& game){
    for(auto& x: items){
        x->use(game);
    }
    items.clear();


}
