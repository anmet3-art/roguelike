#pragma once
#include <memory>
#include <random>
#include <string>
#include <vector>

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
    Game();
    Point findFreeCell(int minDistance);
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
