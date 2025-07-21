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

#ifndef NARTHENIAN_NPC_H
#define NARTHENIAN_NPC_H

#include <string>
#include <vector>
#include "common.h"

/**
 * @brief A Non-playing character
 * This class represents a character that can talk with the player, and in some cases may give an item or give
 * or update a quest. This class is also used to represent treasure chests.
 */
class NPC {
public:
    NPC();
    NPC(std::string npc_name, int npc_sprite, int npc_x, int npc_y, Direction npc_dir, std::string npc_greeting);
    enum DialogCondition {
        dc_none, dc_prompt, dc_quest_equals, dc_quest_greater_than, dc_quest_less_than
    };
    struct DialogEntry {
        DialogCondition condition = dc_none; // dc_none for always available, otherwise trigger to enable this entry
        std::string prompt = "";  // prompt from user selection or quest name, if required by condition
        int condition_amt = 0;
        std::vector<std::string> lines {};  // The dialog itself
        std::vector<std::string> prompts {};   // A set of replies available to the user
        Item item;  // Item given to player when this dialog entry is shown. Blank name for none.
        std::string update_quest = ""; // Quest to update, blank if none
        int new_quest_status = 0;
        std::string new_quest_entry = "";
    };

    std::string name;
    int sprite;
    int x, y;
    bool can_move = true;
    Direction dir;
    std::vector<DialogEntry> dialog_entries;

    /**
     * Turn NPC to face the player. Does nothing if can_move is false.
     * @param player_dir The direction the player is facing. NPC will turn to the opposite direction.
     */
    void turn(Direction player_dir);
};


#endif //NARTHENIAN_NPC_H
