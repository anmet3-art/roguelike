#pragma once
#include<algorithm>
#include "constans.h"
class Game;
class Entity {
   private:
    int x;
    int y;
    char symbol;
    int hp;
    int maxHp;
    int damage;

   public:
    Entity() {}
    Entity(int x, int y, char symbol, int hp, int damage) {
        this->x = x;
        this->y = y;
        this->hp = hp;
        this->maxHp=hp;
        this->damage = damage;
        this->symbol = symbol;
    }

    // Movement methods(useless for now)
    void moveUp() { y--; }
    void moveDown() { y++; }
    void moveLeft() { x--; }
    void moveRight() { x++; }



    // Getters

    int getX() const { return x; }
    int getY() const { return y; }
    int getHp() const { return hp; }
    int getDamage() const { return damage; }

    void takeDamage(int amount) { hp = hp - amount; }
    void giveHp(int amount){
        hp=hp+amount;
        hp=std::min(hp, maxHp);
    }
    virtual bool isDead() const { return hp <= 0; }

    char getSymbol() const { return symbol; }
    void Newcoord(int newX, int newY) {
        x = newX;
        y = newY;
    }
    virtual void update(Game&) {}
    virtual void interact(Game&){}
    virtual ~Entity() = default;
};

//---------------------Entity------------------------
class Player : public Entity {
   public:
    Player(int x, int y, int hp, int damage) : Entity(x, y, consts::PLAYER_SYMBOL, hp, damage) {}
};

class Enemy : public Entity {
   private:
    int visionRange;

   public:
    Enemy(int x, int y, int hp, int damage, int visionRange) : Entity(x, y, consts::ENEMY_SYMBOL, hp, damage) {
        this->visionRange = visionRange;
    }
    void interact(Game& game) override;
    void update(Game& game) override;
};
