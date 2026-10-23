#pragma once
#include<algorithm>
#include "constants.h"
#include "item.h"
#include<memory>
#include<vector>
class Game;
class Entity {
   private:
    int x{0};
    int y{0};
    char symbol{'0'};
    int hp{0};
    int maxHp{0};
    int damage{0};

   public:
    Entity(int x, int y, char symbol, int hp, int damage) {
        this->x = x;
        this->y = y;
        this->hp = hp;
        this->maxHp=hp;
        this->damage = damage;
        this->symbol = symbol;
    }
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
    void giveDamage(int amount){
        damage=damage+amount;
    }
    virtual bool isDead() const { return hp <= 0; }

    char getSymbol() const { return symbol; }
    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
    virtual void update(Game&) {}
    virtual void interact(Game&){}
    virtual ~Entity() = default;
};

//---------------------Entity------------------------
class Player : public Entity {
    private:
    std::vector<std::unique_ptr<Item>> playerInventory;

   public:
   void pushItem(std::unique_ptr<Item> item);
   bool hasItem() const;
   int getItemCount() const;
   std::unique_ptr<Item> takeItem();
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
