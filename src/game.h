#pragma once
#include <memory>
#include <random>
#include <string>
#include <vector>
#include "chest.h"
#include "constants.h"
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
    bool won {false};
    std::mt19937 generator{std::random_device{}()};
    std::vector<Point> freeCells;
    int currentLevel{1};

    std::vector<std::vector<int>> distToPlayer;
   public:
    Game();
    void startLevel();
    Point findFreeCell(int minDistance);
    int randomInt(int from, int to) {
        std::uniform_int_distribution<int> dice(from, to);
        int roll = dice(generator);
        return roll;
    }

    std::unique_ptr<Item> makeRandomItem() {
        int itemType=randomInt(1,2);
        switch (itemType){
        case 1:
            return std::make_unique<HealPotion>(randomInt(consts::HEAL_POTION_MIN, consts::HEAL_POTION_MAX));
        case 2:
            return std::make_unique<PowerPotion>(randomInt(consts::POWER_POTION_MIN, consts::POWER_POTION_MAX));
        default:
            return std::make_unique<HealPotion>(randomInt(consts::HEAL_POTION_MIN, consts::HEAL_POTION_MAX));
        }
    }

    //-------------------------functions-------------------------
    void computeDistances();
    Point nextStepToPlayer(int x, int y) const;
    bool isWalkable(int x, int y) const {  // есть ли в точке пребывания другой объект
        if (y < 0 || y >= static_cast<int>(map.size()) || x < 0 ||
            x >= static_cast<int>(map[y].size())) {
            return false;
        }
        return map[y][x] != '#' && entityAt(x, y) == nullptr;
    }
    bool loadMap(const std::string& level);
    void healPlayer(int amount) { player->giveHp(amount); }
    void boostPlayerDamage(int amount){player->giveDamage(amount);}
    void damagePlayer(int amount) { player->takeDamage(amount); }
    bool goToNextLevel();
    bool isFloor(int x, int y) const;
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

    Player& getPlayer() const { return *player; }
    //-------------------------ОТРИСОВКА КАРТЫ-----------------------------
    void draw() const;
    //-------------------------ХОД ИГРОКА-----------------------------------
    bool playerTurn();

    //------------------------КОМПАНОВЩИК----------------------------------
    void run();
};
