/* Copyright (C) 2025, Jeremiah Stoddard
 *
 * This file is part of The Narthenian Conspiracy.
 *
 * The Narthenian Conspiracy is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * The Narthenian Conspiracy is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with The Narthenian Conspiracy. If not, see
 * <https://www.gnu.org/licenses/>.
 */

//
// Created by jeremiah on 7/19/25.
//

#ifndef NARTHENIAN_COMMON_H
#define NARTHENIAN_COMMON_H

#include <string>
#include <vector>
#include <random>
#include <filesystem>

extern std::filesystem::path asset_dir;
extern std::filesystem::path save_dir;

extern std::mt19937 re;

enum Direction {dir_down = 0, dir_left = 4, dir_right = 8, dir_up = 12};

struct Item {
    enum ItemType { item_unequippable, item_headwear, item_armor, item_footwear, item_shield, item_onehanded,
        item_two_handed, item_necklace, item_ring };
    enum ItemEffect { effect_none, effect_restore_hp, effect_restore_mp };
    std::string name = "none";
    int value = 0;
    ItemType item_type = item_unequippable;

    // Boosts provided by equippable items
    int atk_boost = 0;
    int def_boost = 0;
    int hp_boost = 0;
    int mp_boost = 0;
    int int_boost = 0;

    // What happens when item is used
    ItemEffect effect = effect_none;
    int effect_magnitude = 0;
    bool consumed_on_use = true;
};

struct Quest {
    std::string name;
    int status;
    std::vector<std::string> journal;
};

#endif //NARTHENIAN_COMMON_H
