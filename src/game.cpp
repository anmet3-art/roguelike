#include "game.h"
#include "item.h"
#include <conio.h>

#include <cstdlib>
#include <iostream>

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
    std::cout << "Hp:" << hpplayer << std::endl;
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
    }
    Entity* playerMoveTarget = entityAt(nx, ny);
    if (playerMoveTarget != nullptr && playerMoveTarget != player) {
        playerMoveTarget->interact(*this);
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
    }
    if (!hasEnemies()) {
        std::cout << "EXELENT!" << std::endl << "YOU ARE WIN!" << std::endl;
    } else {
        std::cout << "END GAME!" << std::endl;
    }
    _getch();
}
