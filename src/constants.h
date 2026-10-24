#pragma once

// Константы проекта
namespace consts {

// Игрок
constexpr int PLAYER_START_X = 1;
constexpr int PLAYER_START_Y = 1;

constexpr int PLAYER_HP = 20;
constexpr int PLAYER_HP_MAX = 20;
constexpr int PLAYER_DAMAGE = 2;
constexpr char PLAYER_SYMBOL = '@';

constexpr int INVENTORY_MAX_ITEMS = 2;
// Враги
constexpr int ENEMY_COUNT_MIN = 6;
constexpr int ENEMY_COUNT_MAX = 7;

constexpr int ENEMY_HP_MIN = 4;
constexpr int ENEMY_HP_MAX = 6;

constexpr int ENEMY_DAMAGE_MIN = 2;
constexpr int ENEMY_DAMAGE_MAX = 4;

constexpr int ENEMY_VISION_RANGE = 6;
constexpr int ENEMY_MIN_SPAWN_DISTANCE = 8;
constexpr char ENEMY_SYMBOL = 'M';

// Сундуки
constexpr int CHEST_COUNT_MIN = 3;
constexpr int CHEST_COUNT_MAX = 6;

constexpr int CHEST_MIN_SPAWN_DISTANCE = 0;
constexpr int CHEST_HP = 1;
constexpr int CHEST_DAMAGE = 0;
constexpr char CHEST_SYMBOL = 'C';

// Лестница
constexpr char STAIRS_SYMBOL='>';

// Предметы
constexpr int HEAL_POTION_MIN = 10;
constexpr int HEAL_POTION_MAX = 10;

constexpr int POWER_POTION_MIN = 1;
constexpr int POWER_POTION_MAX = 1;
// Служебное
constexpr int NO_CELL = -1;  // «свободной клетки нет»
}  // namespace consts
