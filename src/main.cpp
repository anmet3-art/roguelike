#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "entity.h"

class Game;



//----------------------Main class-------------------------
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
        auto player_owner = std::make_unique<Player>(1, 1, 20, 5);
        player = player_owner.get();
        entities.push_back(std::move(player_owner));
        entities.push_back(std::make_unique<Enemy>(10, 10, 5, 2, 4));  // x y hp dmg visionRange
        entities.push_back(std::make_unique<Enemy>(10, 11, 5, 2, 4));
        entities.push_back(std::make_unique<Enemy>(11, 10, 5, 2, 5));
        entities.push_back(std::make_unique<Enemy>(11, 11, 5, 2, 6));
    }

    //-------------------------functions-------------------------
    bool isWalkable(int x, int y) const {  // есть ли в точке пребывания другой объект
        if (y < 0 || y >= static_cast<int>(map.size()) || x < 0 ||
            x >= static_cast<int>(map[y].size())) {
            return false;
        }
        return map[y][x] == '.' && entityAt(x, y) == nullptr;
    }
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
    void draw() const {
        system("cls");
        int my = -1, mx = -1;
        for (const std::string& row : map) {
            my += 1;
            mx = -1;
            for (char c : row) {
                mx += 1;
                Entity* entity_at = entityAt(mx, my);
                if (entity_at != nullptr) {
                    std::cout << entity_at->getSymbol();
                } else {
                    std::cout << c;
                }
            }
            std::cout << std::endl;
        }
        int hpplayer = (player->getHp());
        std::cout << "Hp:" << hpplayer << std::endl;
    };
    //-------------------------ХОД ИГРОКА-----------------------------------
    bool playerTurn() {
        char input = _getch();
        bool turn = 1;
        int nx = 0, ny = 0;
        nx = player->getX();
        ny = player->getY();
        if (input == 'w') {
            nx = player->getX();
            ny = player->getY() - 1;
        } else if (input == 's') {
            nx = player->getX();
            ny = player->getY() + 1;

        } else if (input == 'a') {
            nx = player->getX() - 1;
            ny = player->getY();

        } else if (input == 'd') {
            nx = player->getX() + 1;
            ny = player->getY();
        }
        Entity* playerMoveTarget = entityAt(nx, ny);
        if (playerMoveTarget != nullptr && playerMoveTarget != player) {
            playerMoveTarget->takeDamage(player->getDamage());
            turn = true;

        } else if (isWalkable(nx, ny)) {
            player->Newcoord(nx, ny);
            turn = true;

        } else {
            turn = false;
        }

        if (input == 'q') {
            running = false;
        }
        return turn;
    }

    //------------------------КОМПАНОВЩИК----------------------------------
    void run() {
        while (running) {
            draw();
            bool turn = playerTurn();
            std::erase_if(entities, [this](const auto& ent) {
                return (ent->isDead() && ent.get() != player);
            });
            if (!hasEnemies()) {
                running = false;
            }
            if (turn) {
                for (auto& ent : entities) {
                    ent->update(*this);
                }
            }
            if (player->isDead()) {
                running = false;
            }
        }
        draw();

        if (player->isDead()) {
            std::cout << "YOU ARE DEAD!" << std::endl;
        }
        if (!hasEnemies()) {
            std::cout << "EXELENT!" << std::endl << "YOU ARE WIN!" << std::endl;
        } else {
            std::cout << "END GAME!" << std::endl;
        }
        _getch();
    }
};

//-------------------MOVE ENEMY----------------------
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

int main() {
    Game game;
    game.run();
    return 0;
}
