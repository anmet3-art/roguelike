#pragma once
#include <conio.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

#include "chest.h"
#include "entity.h"
struct Point {
    int x = 0;
    int y = 0;
};

class Game {
   private:
    std::vector<std::string> map;
    std::vector<std::unique_ptr<Entity>> entities;
    Player* player{nullptr};
    bool running = true;
    std::mt19937 generator{std::random_device{}()};
    std::vector<Point> freeCells;

   public:
    Point findFreeCell(int minDistance);
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
        entities.push_back(std::move(player_owner));  // x  y      hp              dmg visionRange

        for (int y = 0; y < static_cast<int>(map.size()); y += 1) {
            for (int x = 0; x < static_cast<int>(map[0].size()); x += 1) {
                if (isWalkable(x, y)) {
                    Point p;
                    p.x = x;
                    p.y = y;
                    freeCells.push_back(p);
                }
            }
        }
        std::shuffle(freeCells.begin(), freeCells.end(), generator);
        // Enemy
        int enemyCount = randomInt(3, 6);
        for (int i = 0; i < enemyCount; i += 1) {
            Point p = findFreeCell(8); // В скобках=минимальное расстояние от игрока до монстра
            if(p.x!=-1){
                entities.push_back(std::make_unique<Enemy>(p.x, p.y, randomInt(4, 6), randomInt(1, 4), 7));
            } else{
                break;
            }
        }
        // Chest
        int chestCount = randomInt(2, 4);
        for (int i = 0; i < chestCount; i += 1) {
            Point p = findFreeCell(0);
            if(p.x!=-1){
                entities.push_back(std::make_unique<Chest>(p.x, p.y, makeRandomItem()));
            } else{
                break;
            }
        }
    }
}

    int randomInt(int from, int to) {
        std::uniform_int_distribution<int> dice(from, to);
        int roll = dice(generator);
        return roll;
    }

    std::unique_ptr<Item> makeRandomItem() {
        return std::make_unique<HealPotion>(randomInt(3, 10));
    }

    //-------------------------functions-------------------------
    bool isWalkable(int x, int y) const {  // есть ли в точке пребывания другой объект
        if (y < 0 || y >= static_cast<int>(map.size()) || x < 0 ||
            x >= static_cast<int>(map[y].size())) {
            return false;
        }
        return map[y][x] == '.' && entityAt(x, y) == nullptr;
    }

    void healPlayer(int amount) { player->giveHp(amount); }
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
