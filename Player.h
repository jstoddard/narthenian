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

#ifndef NARTHENIAN_PLAYER_H
#define NARTHENIAN_PLAYER_H

#include <string>
#include <vector>
#include "Combatant.h"
#include "common.h"

/**
 * Basic data for the player
 */
class Player {
public:
    int x, y;
    Direction dir;
    int sprite = 0;
    std::vector<Item> inventory;
    Combatant combatant;
    Item headwear, armor, footwear, shield, weapon, necklace, ring;

    Player();
    /**
     * @brief Add experience
     * Add to the player's experience. This method is preferred over directly increasing exp in the combatant
     * member because it caps the experience at 100,000, preventing it from eventually overflowing.
     * @param additional_exp the amount of experience to add
     */
     void add_experience(int additional_exp);

    /**
     * @brief Increase the player's level
     * Set level to the max level x such that x*x*x <= exp, and adjust stats accordingly.
     */
    void level_up();
};

#endif //NARTHENIAN_PLAYER_H
