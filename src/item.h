#pragma once
class Game;
class Item {
   private:
   public:
    virtual void use(Game& game) = 0;
    virtual ~Item() = default;
};
class HealPotion : public Item {
   private:
    int healpoint;

   public:
    HealPotion(int healpoint) { this->healpoint = healpoint; }
    void use(Game& game) override;
};
class PowerPotion : public Item {
   private:
    int damageIncrease;

   public:
    PowerPotion(int damageIncrease) { this->damageIncrease = damageIncrease; }
    void use(Game& game) override;
}

