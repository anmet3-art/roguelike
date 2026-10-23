#include "game.h"

#include <conio.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "constants.h"
#include "item.h"

void Game::draw() const {
    std::string frame = "\033[H";
    int my = -1, mx = -1;
    for (const std::string& row : map) {
        my += 1;
        mx = -1;
        for (char c : row) {
            mx += 1;
            Entity* entity_at = entityAt(mx, my);
            if (entity_at != nullptr) {
                frame += entity_at->getSymbol();
            } else {
                frame += c;
            }
        }
        frame += "\n";
    }
    frame += "\033[J";
    std::cout << frame;
    int hpplayer = (player->getHp());
    std::cout << "Hp: " << hpplayer<< '/' << consts::PLAYER_HP_MAX<< std::endl;
    std::cout<<"Items: " << player->getItemCount() << std::endl;
}

bool Game::playerTurn() {
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
    } else if (input == '1') {
        if(player->hasItem()){
            player->takeItem()->use(*this);
            turn=true;
            return turn;
        }
        else{
            turn=false;
            return turn;
        }
    }
    Entity* playerMoveTarget = entityAt(nx, ny);
    if (playerMoveTarget != nullptr && playerMoveTarget != player) {
        playerMoveTarget->interact(*this);
        turn = true;

    } else if (isWalkable(nx, ny)) {
        player->setPosition(nx, ny);
        turn = true;

    } else {
        turn = false;
    }

    if (input == 'q') {
        running = false;
    }
    return turn;
}

void Game::run() {
    system("cls");
    while (running) {
        draw();
        bool turn = playerTurn();
        std::erase_if(entities,
                      [this](const auto& ent) { return (ent->isDead() && ent.get() != player); });
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
    } else if (!hasEnemies()) {
        std::cout << "EXELENT!" << std::endl << "YOU ARE WIN!" << std::endl;
    } else {
        std::cout << "END GAME!" << std::endl;
    }
    _getch();
}
Point Game::findFreeCell(int minDistance) {
    int x = player->getX();
    int y = player->getY();
    int dx, dy;
    for (int i = 0; i < static_cast<int>(freeCells.size()); i += 1) {
        Point p = freeCells[i];
        dx = abs(x - p.x);
        dy = abs(y - p.y);
        if ((dx + dy) >= minDistance) {
            freeCells[i] = freeCells.back();
            freeCells.pop_back();
            return p;
        }
    }
    Point p;
    p.x = consts::NO_CELL;
    p.y = consts::NO_CELL;
    return p;
}
Game::Game() {
    map = {"####################", "#..................#", "#..................#",
           "#..................#", "#..................#", "#..................#",
           "#..................#", "#..................#", "#..................#",
           "#..................#", "#..................#", "#..................#",
           "#..................#", "#..................#", "#..................#",
           "#..................#", "#..................#", "#..................#",
           "#..................#", "####################"};
    auto player_owner = std::make_unique<Player>(consts::PLAYER_START_X, consts::PLAYER_START_Y,
                                                 consts::PLAYER_HP, consts::PLAYER_DAMAGE);
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
    int enemyCount = randomInt(consts::ENEMY_COUNT_MIN, consts::ENEMY_COUNT_MAX);
    for (int i = 0; i < enemyCount; i += 1) {
        Point p =
            findFreeCell(consts::ENEMY_MIN_SPAWN_DISTANCE);  // В скобках=минимальное расстояние от
                                                             // игрока до монстра
        if (p.x != consts::NO_CELL) {
            entities.push_back(std::make_unique<Enemy>(
                p.x, p.y, randomInt(consts::ENEMY_HP_MIN, consts::ENEMY_HP_MAX),
                randomInt(consts::ENEMY_DAMAGE_MIN, consts::ENEMY_DAMAGE_MAX),
                consts::ENEMY_VISION_RANGE));
        } else {
            break;
        }
    }
    // Chest
    int chestCount = randomInt(consts::CHEST_COUNT_MIN, consts::CHEST_COUNT_MAX);
    for (int i = 0; i < chestCount; i += 1) {
        Point p = findFreeCell(consts::CHEST_MIN_SPAWN_DISTANCE);
        if (p.x != consts::NO_CELL) {
            entities.push_back(std::make_unique<Chest>(p.x, p.y, makeRandomItem()));
        } else {
            break;
        }
    }
}
