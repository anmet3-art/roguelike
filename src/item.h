#pragma once
#include<string>
class Game;
class Item {
   private:
   public:
    virtual void use(Game& game) = 0;
    virtual std::string getName() const =0;
    virtual ~Item() = default;
};
class HealPotion : public Item {
   private:
    int healpoint;

   public:
    HealPotion(int healpoint) { this->healpoint = healpoint; }
    std::string getName() const override;
    void use(Game& game) override;
};
class PowerPotion : public Item {
   private:
    int damageIncrease;

   public:
    PowerPotion(int damageIncrease) { this->damageIncrease = damageIncrease; }
    void use(Game& game) override;
    std::string getName() const override;
};

