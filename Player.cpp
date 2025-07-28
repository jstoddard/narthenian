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
// Created by jeremiah on 7/15/25.
//

#include <random>
#include <cmath>
#include "Player.h"
#include "common.h"

Player::Player() {
    combatant.name = "Player";
}

void Player::level_up() {
    int new_lvl = (int)std::cbrt(combatant.exp);
    int increase = new_lvl - combatant.lvl;
    if (increase < 1)
        return;
    std::uniform_int_distribution<int> ud {1, 5};
    for (int i = 0; i < increase; i++)
    {
        combatant.strength += ud(re);
        combatant.resilience += ud(re);
        combatant.dexterity += ud(re);
        combatant.intelligence += ud(re);
        combatant.max_hp += ud(re);
        combatant.max_mp += ud(re);
    }
    combatant.lvl = new_lvl;
    combatant.hp = combatant.max_hp;
    combatant.mp = combatant.max_mp;
}

void Player::add_experience(int additional_exp) {
    combatant.exp += additional_exp;
    if (combatant.exp > 100000)
        combatant.exp = 100000;
}
