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

#include "NPC.h"

#include <utility>

NPC::NPC() {

}

NPC::NPC(std::string npc_name, int npc_sprite, int npc_x, int npc_y, Direction npc_dir, std::string npc_greeting) {
    name = std::move(npc_name);
    sprite = npc_sprite;
    x = npc_x;
    y = npc_y;
    dir = npc_dir;
    DialogEntry d;
    d.condition = dc_none;
    d.lines = { std::move(npc_greeting) };
    dialog_entries.push_back(d);
}

void NPC::turn(Direction player_dir) {
    if (can_move) {
        switch (player_dir) {
            case dir_up:
                dir = dir_down;
                break;
            case dir_down:
                dir = dir_up;
                break;
            case dir_left:
                dir = dir_right;
                break;
            case dir_right:
                dir = dir_left;
                break;
        }
    }
}
