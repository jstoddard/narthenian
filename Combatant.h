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
// Created by jeremiah on 6/19/25.
//

#ifndef NARTHENIAN_COMBATANT_H
#define NARTHENIAN_COMBATANT_H

#include <string>

/**
 * Player or monster. This class holds the attributes of entities that fight.
 */
struct Combatant {
    std::string name;
    int max_hp = 5, max_mp = 5;
    int hp = 5, mp = 5;
    int strength = 5, resilience= 3, intelligence = 0, dexterity = 1;
    // TODO: Make these separate types
    int weapon = 0;
    int armor = 0;
    // TODO: Spells and effects -- need classes
    // std::vector<Spell> spells;
    // std::vector<Effects> active_effects;
    int exp = 0;
    int lvl = 1;
    int gold = 0;
    bool is_player = false;
    [[nodiscard]] std::string name_with_article() const;
};


#endif //NARTHENIAN_COMBATANT_H
