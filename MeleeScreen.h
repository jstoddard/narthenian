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
// Created by jeremiah on 6/17/25.
//

#ifndef NARTHENIAN_MELEESCREEN_H
#define NARTHENIAN_MELEESCREEN_H

#include <string>
#include <random>
#include <unordered_map>

#include "Screen.h"
#include "GameScreen.h"
#include "MessageBox.h"
#include "Menu.h"
#include "Combatant.h"

class MeleeScreen : public Screen {
private:
    std::unordered_map<std::string, SDL_Texture*> monster_textures;
    std::vector<SDL_Texture *> bg_textures;
    std::mt19937 re;
    MessageBox stats_box;
    MessageBox message_box;
    Menu menu;
    int bg = 0;
    int animate_count = 0;
    enum States {
        melee_none,
        melee_showing_menu,
        melee_player_turn,
        melee_monster_turn,
        melee_wait_ack,
        melee_player_won,
        melee_player_died,
        melee_check_level_up,
        melee_over
    };
    States cur_state = melee_none;
    States prev_state = melee_none;
    enum Actions {
        melee_deciding,
        melee_fight,
        melee_spell,
        melee_casting,
        melee_defend,
        melee_flee,
        melee_done
    };
    Actions combatant_action = melee_fight;
    Combatant enemy;
    std::unordered_map<std::string, Combatant> enemies;
    int wait_frames = 0;
    void do_menu();
    void do_player_turn();
    void do_monster_turn();
    void init_monsters();
    void set_enemy(const std::string& e);
    void select_enemy(std::vector<std::string> options);
    void set_state(States new_state);
    void attack(Combatant& attacker, Combatant& defender);
    bool can_flee(Combatant& escapee, Combatant& pursuer);
    void check_level_up();
public:
    MeleeScreen();
    ~MeleeScreen();
    void iterate() override;
    void draw(int frame) override;
    void init_melee(int zone);
};


#endif //NARTHENIAN_MELEESCREEN_H
