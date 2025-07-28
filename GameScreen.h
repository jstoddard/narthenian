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
// Created by jeremiah on 6/12/25.
//

#ifndef NARTHENIAN_GAMESCREEN_H
#define NARTHENIAN_GAMESCREEN_H

#include <string>
#include <unordered_map>
#include <random>
#include "Screen.h"
#include "AreaMap.h"
#include "Combatant.h"
#include "Player.h"
#include "common.h"

/**
 * @brief The game screen
 * This class provides the basic user interface for an ongoing game while a player is exploring one of the game maps,
 * allowing the player to move around the map and enter other maps. Other actions, such as dialog and combat, are
 * handled by other Screen subclasses.
 */
class GameScreen : public Screen {
private:
    std::unordered_map<std::string, AreaMap*> maps {};
    AreaMap *cur_map;
    enum States {
        game_waiting, game_delay_input,
        game_moving_up, game_moving_down, game_moving_left, game_moving_right,
        game_check_tile
    };
    States cur_state = game_waiting;
    int player_dst, player_progress;    // For animating player movement
    SDL_Texture *sprites;
    int delay_frames = 0;

    void init_portals();
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void inc_move_up();
    void inc_move_down();
    void inc_move_left();
    void inc_move_right();
    bool precheck_tile(int x, int y);
    void check_tile();
    void draw_sprite(int frame, int sprite, int x, int y, int x_start, int y_start, Direction dir);
    void talk();
    void bump();

public:
    GameScreen();
    ~GameScreen();
    void new_game();
    void iterate() override;
    void draw(int frame) override;
    Player player;

    void init_npcs();
};


#endif //NARTHENIAN_GAMESCREEN_H
