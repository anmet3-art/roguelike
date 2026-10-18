#pragma once
#include <conio.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
// kaggle = money
#include "entity.h"
#include "chest.h"

class Game {
   private:
    std::vector<std::string> map;
    std::vector<std::unique_ptr<Entity>> entities;
    Player* player{nullptr};
    bool running = true;

   public:
    Game() {
        map = {"####################", "#..................#", "#..................#",
               "#..................#", "#..................#", "#..................#",
               "#..................#", "#..................#", "#..................#",
               "#..................#", "#..................#", "#..................#",
               "#..................#", "#..................#", "#..................#",
               "#..................#", "#..................#", "#..................#",
               "#..................#", "####################"};
        auto player_owner = std::make_unique<Player>(1, 1, 20, 2);
        player = player_owner.get();
        entities.push_back(std::move(player_owner));
        entities.push_back(std::make_unique<Enemy>(10, 10, 6, 2, 7));  // x y hp dmg visionRange
        entities.push_back(std::make_unique<Enemy>(11, 11, 6, 2, 8));
        entities.push_back(std::make_unique<Enemy>(9, 11, 6, 2, 9));
        entities.push_back(std::make_unique<Enemy>(10, 12, 6, 2, 10));
        entities.push_back(std::make_unique<Chest>(4, 4));
        entities.push_back(std::make_unique<Chest>(8, 4));
    }

    //-------------------------functions-------------------------
    bool isWalkable(int x, int y) const {  // есть ли в точке пребывания другой объект
        if (y < 0 || y >= static_cast<int>(map.size()) || x < 0 ||
            x >= static_cast<int>(map[y].size())) {
            return false;
        }
        return map[y][x] == '.' && entityAt(x, y) == nullptr;
    }

    void healPlayer(int amount){ player->giveHp(amount);}
    void damagePlayer(int amount) { player->takeDamage(amount); }


    Entity* entityAt(int x, int y) const {  // Что это за существо
        for (const auto& ent : entities) {
            if (x == ent->getX() && y == ent->getY()) {
                return ent.get();
            }
        }
        return nullptr;
    }
    bool hasEnemies() const {
        for (const auto& enem : entities) {
            if (dynamic_cast<Enemy*>(enem.get()) != nullptr) {
                return true;
            }
        }
        return false;
    }

    const Player& getPlayer() const { return *player; }
    //-------------------------ОТРИСОВКА КАРТЫ-----------------------------
    void draw() const;
    //-------------------------ХОД ИГРОКА-----------------------------------
    bool playerTurn();

    //------------------------КОМПАНОВЩИК----------------------------------
    void run();
};
