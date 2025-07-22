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

#include "GameScreen.h"
#include "MeleeScreen.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Sound.h"
#include "common.h"
#include "DialogScreen.h"

extern int window_h, window_w;
extern int draw_h, draw_w;
extern SDL_Renderer *renderer;
extern Sound *sound;
extern Screen *cur_screen;
extern MeleeScreen *melee_screen;
extern DialogScreen *dialog_screen;

GameScreen::GameScreen() {
    maps.insert({
        {"overworld", new AreaMap(asset_dir / "tileset.png", asset_dir / "overworld.map")},
        {"carathusia", new AreaMap(asset_dir / "tileset.png", asset_dir / "carathusia.map")}
    });
    sprites = IMG_LoadTexture(renderer, (asset_dir / "sprites.png").c_str());
    SDL_SetTextureScaleMode(sprites, SDL_SCALEMODE_NEAREST);

    init_portals();
    init_npcs();

    new_game();
}

GameScreen::~GameScreen() {
    for (auto m: maps)
    {
        delete m.second;
    }
    SDL_DestroyTexture(sprites);
}

void GameScreen::init_portals() {
    // TODO: Move this stuff into a data file. The more outsourced to data files, the easier it will be to modify
    //  the game or make different games without editing the program code.

    // Overworld Portals
    auto& op = maps.find("overworld")->second->portals;
    op.push_back({"carathusia", 95, 187, 25, 1, 48, 25, 1, 25, 25, 48});

    // Carathusia Portals
    auto& cp = maps.find("carathusia")->second->portals;
    cp.push_back({"overworld", 25, 0, 95, 187, 95, 187, 95, 187, 95, 187});
    cp.push_back({"overworld", 49, 25, 95, 187, 95, 187, 95, 187, 95, 187});
    cp.push_back({"overworld", 0, 25, 95, 187, 95, 187, 95, 187, 95, 187});
    cp.push_back({"overworld", 25, 49, 95, 187, 95, 187, 95, 187, 95, 187});
}


void GameScreen::init_npcs() {
    // TODO: Same as init_portals, load NPC data from a data file.

    // Carathusia NPCs.
    // 14, 6 (inn); 24,24 (by tree in center of town)
    auto& cnpc = maps.find("carathusia")->second->npcs;
    cnpc.emplace_back("Brenda", 2, 24, 24, dir_down, "Welcome to Carathusia!");
    auto mark = NPC("Mark", 1, 14, 6, dir_down, "I am from Dasos, the forest town in");
    mark.dialog_entries[0].lines.emplace_back("the northwest. We have been troubled");
    mark.dialog_entries[0].lines.emplace_back("by powerful monsters lately.");
    cnpc.push_back(mark);
}

void GameScreen::new_game() {
    // Set map to overworld, place player in default position
    cur_map = maps.find("overworld")->second;
    player.x = 95;
    player.y = 186;
    player.dir = dir_down;
    player_dst = player_progress = 0; // Not used until player moves

    // Set some player attributes
    std::normal_distribution nd {15.0, 3.0};
    player.combatant.is_player = true;
    player.combatant.name = "Player";
    player.combatant.max_hp = (int)nd(re);
    if (player.combatant.max_hp < 10)
        player.combatant.max_hp = 10;
    player.combatant.hp = player.combatant.max_hp;
    player.combatant.max_mp = (int)nd(re);
    if (player.combatant.max_mp < 0)
        player.combatant.max_mp = 0;
    player.combatant.mp = player.combatant.max_mp;
    player.combatant.exp = 0;
    player.combatant.lvl = 1;

    std::uniform_int_distribution<int> ud {3, 7};
    player.combatant.strength = ud(re);
    player.combatant.resilience = ud(re);
    player.combatant.intelligence = ud(re);

    // Reset objects, quests, etc.
}

void GameScreen::iterate() {
    // Give player a chance to stop doing something
    if (cur_state == game_waiting)
    {
        Command cmd = get_cmd();
        switch(cmd)
        {
            case CMD_UP:
                move_up();
                break;
            case CMD_DOWN:
                move_down();
                break;
            case CMD_LEFT:
                move_left();
                break;
            case CMD_RIGHT:
                move_right();
                break;
            case CMD_ACTION1:
                talk();
                break;
            case CMD_NONE:
            default:
                // Do nothing
                break;
        }
    }

    // What should we do?
    switch (cur_state)
    {
        case game_delay_input:
            if (delay_frames < 1) {
                delay_frames = 0;
                cur_state = game_waiting;
            }
            else {
                delay_frames--;
            }
            break;
        case game_moving_up:
            inc_move_up();
            break;
        case game_moving_down:
            inc_move_down();
            break;
        case game_moving_left:
            inc_move_left();
            break;
        case game_moving_right:
            inc_move_right();
            break;
        case game_check_tile:
            check_tile();
            break;
        case game_waiting:
            break;
    }
}

void GameScreen::draw(int frame) {
    SDL_FRect src, dst;

    // Draw player in middle of screen (unless we find out he's close to the map edge)
    int draw_player_x = window_w / 2 - draw_w / 2;
    int draw_player_y = window_h / 2 - draw_h / 2;

    /* start drawing map at half screen above and left of player, minimum
     * (0,0); maximum (map width-screen_w, map height-screen_h)
    */
    int x_start = 16*(player.x - (window_w/draw_w)/2) + 8;
    int y_start = 16*(player.y - (window_h/draw_h)/2);

    // If player is moving, adjust drawing location
    switch(cur_state)
    {
        case game_moving_up:
            y_start -= player_progress;
            break;
        case game_moving_down:
            y_start += player_progress;
            break;
        case game_moving_left:
            x_start -= player_progress;
            break;
        case game_moving_right:
            x_start += player_progress;
            break;
        default:
            break;
    }

    if (x_start < 0) // player is close to left edge of map
    {
        x_start = 0;
        draw_player_x = player.x * draw_w;
        if (cur_state == game_moving_left)
            draw_player_x -= (int)((float)draw_w/16)*player_progress;
        if (cur_state == game_moving_right)
            draw_player_x += (int)((float)draw_w/16)*player_progress;
    }
    if (x_start + 16*(window_w/draw_w) > 16 * cur_map->width()) // player is close to right edge of map
    {
        x_start = 16 * cur_map->width() - 16 * (window_w / draw_w);
        draw_player_x = window_w - draw_w*(cur_map->width() - player.x);
        if (cur_state == game_moving_left)
            draw_player_x -= (int)((float)draw_w/16)*player_progress;
        if (cur_state == game_moving_right)
            draw_player_x += (int)((float)draw_w/16)*player_progress;
    }
    if (y_start < 0) // player is close to top of map
    {
        y_start = 0;
        draw_player_y = player.y * draw_h;
        if (cur_state == game_moving_up)
            draw_player_y -= (int)((float)draw_h/16)*player_progress;
        if (cur_state == game_moving_down)
            draw_player_y += (int)((float)draw_h/16)*player_progress;
    }
    if (y_start + 16*(window_h/draw_h) > 16 * cur_map->height()) // player is close to bottom of map
    {
        y_start = 16 * cur_map->height() - 16 * (window_h / draw_h);
        draw_player_y = window_h - draw_h*(cur_map->height() - player.y);
        if (cur_state == game_moving_up)
            draw_player_y -= (int)((float)draw_h/16)*player_progress;
        if (cur_state == game_moving_down)
            draw_player_y += (int)((float)draw_h/16)*player_progress;
    }

    // draw current map
    cur_map->draw(x_start, y_start, frame);

    // draw player
    src.w = 16;
    src.h = 16;
    src.x = (float)(16*(player.dir + frame));
    src.y = 0;
    dst.w = (float)draw_w;
    dst.h = (float)draw_h;
    dst.x = (float)draw_player_x;
    dst.y = (float)draw_player_y;
    SDL_RenderTexture(renderer, sprites, &src, &dst);

    // draw npcs
    for (const auto& npc : cur_map->npcs) {
        int x = npc.x * 16;
        int y = npc.y * 16;
        if (x + 16 > x_start && x < x_start + 17*(window_w/draw_w)
            && y + 16 > y_start && y < y_start + 16*(window_h/draw_h)) {
            SDL_FRect src;
            SDL_FRect dst;
            src.x = (float)(16 * (npc.dir + frame));
            src.y = (float)(16 * npc.sprite);
            src.w = 16;
            src.h = 16;
            dst.x = (float)((x - x_start)) * ((float)draw_w/16);
            dst.y = (float)((y - y_start)) * ((float)draw_h/16);
            dst.w = (float)draw_w;
            dst.h = (float)draw_h;
            SDL_RenderTexture(renderer, sprites, &src, &dst);
        }
    }
}

bool GameScreen::precheck_tile(int x, int y) {
    if (y < 0 || y >= cur_map->height() || x < 0 || x >= cur_map->width() || cur_map->zone(x, y) == 0) {
        bump();
        return false;
    }
    for (const auto& npc : cur_map->npcs) {
        if (npc.x == x && npc.y == y) {
            bump();
            return false;
        }
    }
    return true;
}

void GameScreen::move_up() {
    player.dir = dir_up;
    int dst_x = player.x;
    int dst_y = player.y - 1;
    if (precheck_tile(dst_x, dst_y))  {
        cur_state = game_moving_up;
        player_dst = player.y - 1;
        player_progress = 0;
    }
}

void GameScreen::move_down() {
    player.dir = dir_down;
    int dst_x = player.x;
    int dst_y = player.y + 1;
    if (precheck_tile(dst_x, dst_y)) {
        cur_state = game_moving_down;
        player_dst = player.y + 1;
        player_progress = 0;
    }
}

void GameScreen::move_left() {
    player.dir = dir_left;
    int dst_x = player.x - 1;
    int dst_y = player.y;
    if (precheck_tile(dst_x, dst_y)) {
        cur_state = game_moving_left;
        player_dst = player.x - 1;
        player_progress = 0;
    }
}

void GameScreen::move_right() {
    player.dir = dir_right;
    int dst_x = player.x + 1;
    int dst_y = player.y;
    if (precheck_tile(dst_x, dst_y)) {
        cur_state = game_moving_right;
        player_dst = player.x + 1;
        player_progress = 0;
    }
}

void GameScreen::inc_move_up() {
    player_progress++;
    if (player_progress > 15)
    {
        player.y = player_dst;
        cur_state = game_check_tile;
    }
}

void GameScreen::inc_move_down() {
    inc_move_up();
}

void GameScreen::inc_move_left() {
    player_progress++;
    if (player_progress > 15)
    {
        player.x = player_dst;
        cur_state = game_check_tile;
    }
}

void GameScreen::inc_move_right() {
    inc_move_left();
}

void GameScreen::check_tile() {
    // Check for a portal on this tile, move to another map if so
    for (auto p : cur_map->portals)
    {
        if (p.x == player.x && p.y == player.y)
        {
            sound->play_steps();
            auto new_map = maps.find(p.dst_map)->second;
            if (!new_map)
            {
                SDL_Log("Map %s not found!", p.dst_map.c_str());
                return;
            }
            cur_map = new_map;
            switch(player.dir)
            {
                case dir_down:
                    player.x = p.down_x;
                    player.y = p.down_y;
                    break;
                case dir_left:
                    player.x = p.left_x;
                    player.y = p.left_y;
                    break;
                case dir_right:
                    player.x = p.right_x;
                    player.y = p.right_y;
                    break;
                case dir_up:
                default:
                    player.x = p.up_x;
                    player.y = p.up_y;
                    break;
            }
            delay_frames = 20;
            cur_state = game_delay_input;
            return;
        }
    }

    // Check for enemies
    int z = cur_map->zone(player.x, player.y);
    if (z > 1)
    {
        std::uniform_int_distribution<int> rnd(0,19);
        // Update later to use c++'s random number facilities
        if (rnd(re) == 0)
        {
            melee_screen->init_melee(z);
            cur_screen = melee_screen;
        }
    }
    cur_state = game_waiting;
}

void GameScreen::bump() {
    // Play a bump sound and delay taking input for a few frames
    sound->play_bump();
    delay_frames = 20;
    cur_state = game_delay_input;
}

void GameScreen::talk() {
    // If there is an NPC directly in front of the player, talk
    int x = player.x;
    int y = player.y;
    switch(player.dir) {
        case dir_up:
            y--;
            break;
        case dir_down:
            y++;
            break;
        case dir_left:
            x--;
            break;
        case dir_right:
            x++;
            break;
    }
    for (auto& npc : cur_map->npcs) {
        if (npc.x == x && npc.y == y) {
            npc.turn(player.dir);
            dialog_screen->set_npc(&npc);
            cur_screen = dialog_screen;
        }
    }
    delay_frames = 20;
    cur_state = game_delay_input;
}
