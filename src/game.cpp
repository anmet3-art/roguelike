#include "game.h"

#include <conio.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>

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
    std::cout << "Hp: " << hpplayer << '/' << consts::PLAYER_HP_MAX
              << " Dmg: " << (player->getDamage()) << std::endl;
    std::cout << "Items(" << player->getItemCount() << '/' << consts::INVENTORY_MAX_ITEMS
              << "): " << player->getItemNames() << std::endl;
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
        if (player->hasItem()) {
            player->takeItem()->use(*this);
            turn = true;
            return turn;
        } else {
            turn = false;
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
        if (map[ny][nx] == consts::STAIRS_SYMBOL && !hasEnemies()) {
            won = !goToNextLevel();
        }

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
        if (won == true) {
            running = false;
        }
        if (turn) {
            computeDistances();
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
    } else if (won == true) {
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
bool Game::loadMap(const std::string& level) {
    std::ifstream file = std::ifstream(level);
    std::string line;
    if (!file.is_open()) {
        return false;
    }
    map.clear();
    while (std::getline(file, line)) {
        map.push_back(line);
    }
    return true;
}
bool Game::goToNextLevel() {
    if (!loadMap("maps/level" + std::to_string(currentLevel + 1) + ".txt")) {
        return false;
    }
    currentLevel += 1;
    startLevel();
    return true;
}

void Game::startLevel() {
    player->setPosition(consts::PLAYER_START_X, consts::PLAYER_START_Y);
    std::erase_if(entities, [this](const auto& ent) { return (ent.get() != player); });
    freeCells.clear();
    for (int y = 0; y < static_cast<int>(map.size()); y += 1) {
        for (int x = 0; x < static_cast<int>(map[0].size()); x += 1) {
            if (isWalkable(x, y) && map[y][x] != consts::STAIRS_SYMBOL) {
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

Game::Game() {
    if (!loadMap("maps/level" + std::to_string(currentLevel) + ".txt")) {
        std::cerr << "cannot open maps/level" + std::to_string(currentLevel) +
                         ".txt, run from project root"
                  << std::endl;
        std::exit(1);
    }
    auto player_owner = std::make_unique<Player>(consts::PLAYER_START_X, consts::PLAYER_START_Y,
                                                 consts::PLAYER_HP, consts::PLAYER_DAMAGE);
    player = player_owner.get();
    entities.push_back(std::move(player_owner));  // x  y  hp   dmg visionRange
    startLevel();
}

bool Game::isFloor(int x, int y) const {
    if (0 <= y && y < static_cast<int>(map.size() - 1) && 0 <= x &&
        x < static_cast<int>(map[y].size() - 1) && map[y][x] != consts::WALLS_SYMBOL &&
        (dynamic_cast<Chest*>(entityAt(x, y)) == nullptr) && distToPlayer[y][x] == -1) {
        return true;
    }
    return false;
}

void Game::computeDistances() {
    distToPlayer = std::vector<std::vector<int>>(map.size(), std::vector<int>(map[0].size(), -1));
    std::queue<Point> q;
    Point p;
    p.x = player->getX();
    p.y = player->getY();
    q.push(p);
    distToPlayer[p.y][p.x] = 0;
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    while (!q.empty()) {
        Point cur;
        cur.x = q.front().x;
        cur.y = q.front().y;
        q.pop();
        for (int i = 0; i < 4; i += 1) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];
            p.x = nx;
            p.y = ny;
            if (isFloor(nx, ny)) {
                distToPlayer[ny][nx] = distToPlayer[cur.y][cur.x] + 1;
                q.push(p);
            }
        }
    }
}
Point Game::nextStepToPlayer(int x, int y) const {
    int minStep;
    Point p;
    p.x = consts::NO_CELL;
    p.y = consts::NO_CELL;
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    minStep = distToPlayer[y][x];
    for (int i = 0; i < 4; i += 1) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (ny >= 0 && ny < static_cast<int>(map.size()) && nx >= 0 &&
            nx < static_cast<int>(map[ny].size())) {
            if (distToPlayer[ny][nx] != -1 && isWalkable(nx, ny)) {
                if (minStep > distToPlayer[ny][nx]) {
                    p.x = nx;
                    p.y = ny;
                    minStep = distToPlayer[ny][nx];
                }
            }
        }
    }
    return p;
}
