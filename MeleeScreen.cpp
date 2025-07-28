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

#include "MeleeScreen.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Sound.h"
#include "common.h"

extern SDL_Renderer *renderer;
extern int window_w, window_h;
extern int draw_w, draw_h;
extern Sound *sound;
extern Screen *cur_screen;
extern GameScreen *game_screen;

MeleeScreen::MeleeScreen()
    :
    menu(draw_w, draw_h, 6, 5, {"Fight", "Spell", "Defend", "Flee"}) {
    // Load image of each monster
    monster_textures.insert({{"serpent", IMG_LoadTexture(renderer, (asset_dir / "serpent.png").c_str())},
                            {"gigarat", IMG_LoadTexture(renderer, (asset_dir / "gigarat.png").c_str())},
                             {"highwayman", IMG_LoadTexture(renderer, (asset_dir / "highwayman.png").c_str())},
                             {"mage", IMG_LoadTexture(renderer, (asset_dir / "mage.png").c_str())}});
    // Set monster textures to scale as pixel art
    for (const auto& t : monster_textures)
        SDL_SetTextureScaleMode(t.second, SDL_SCALEMODE_NEAREST);
    // Load monster stats
    init_monsters();

    // Load the background scenes
    bg_textures.push_back(IMG_LoadTexture(renderer, (asset_dir / "bg0.png").c_str()));
    bg_textures.push_back(IMG_LoadTexture(renderer, (asset_dir / "bg1.png").c_str()));
    bg_textures.push_back(IMG_LoadTexture(renderer, (asset_dir / "bg2.png").c_str()));
    bg_textures.push_back(IMG_LoadTexture(renderer, (asset_dir / "bg3.png").c_str()));
    for (auto t : bg_textures)
        SDL_SetTextureScaleMode(t, SDL_SCALEMODE_NEAREST);

    // Most of this is recalculated in draw, so probably should just move the rest of it there and skip it here
    stats_box.setpos(window_w - 7 * draw_w, draw_h);
    stats_box.setsize(6, 6);
    message_box.setpos(draw_w, window_h - 6 * draw_h);
    message_box.setsize(window_w / draw_w - 2, 5);

    // Seed the random number generator
    std::random_device rd;
    re.seed(rd());
}

MeleeScreen::~MeleeScreen() {
    for (const auto& t : monster_textures)
    {
        SDL_DestroyTexture(t.second);
    }
    for (const auto& t : bg_textures)
    {
        SDL_DestroyTexture(t);
    }
}

void MeleeScreen::iterate() {
    if (wait_frames > 0)
    {
        wait_frames--;
        return;
    }
    switch(cur_state)
    {
        case melee_showing_menu:
            do_menu();
            break;
        case melee_wait_ack: {
            Command cmd = get_cmd();
            if (cmd == CMD_ACTION1 || cmd == CMD_ACTION2) {
                message_box.prompt() = false;
                set_state(prev_state);
            }
            break;
        }
        case melee_player_turn:
            do_player_turn();
            break;
        case melee_monster_turn:
            do_monster_turn();
            break;
        case melee_player_won:
            message_box.add_line("You have vanquished the " + enemy.name + ".");
            message_box.add_line("You receive " + std::to_string(enemy.exp) + " experience and");
            message_box.add_line(std::to_string(enemy.gold) + " gold.");
            game_screen->player.add_experience(enemy.exp);
            game_screen->player.combatant.gold += enemy.gold;
            message_box.prompt() = true;
            set_state(melee_wait_ack);
            prev_state = melee_check_level_up;
            break;
        case melee_player_died:
            if (prev_state == melee_wait_ack) {
                game_screen->new_game();  // set cur_screen to title screen (but we don't have one yet)
                cur_screen = game_screen;
            }
            message_box.add_line("You have died...");
            message_box.prompt() = true;
            set_state(melee_wait_ack);
            break;
        case melee_check_level_up:
            check_level_up();
            break;
        case melee_over:
        case melee_none:
            cur_screen = game_screen;
            break;
    }

    stats_box.clear();
    stats_box.add_line("HP " + std::to_string(game_screen->player.combatant.hp));
    stats_box.add_line("MP " + std::to_string(game_screen->player.combatant.mp));
    stats_box.add_line("Lvl " + std::to_string(game_screen->player.combatant.lvl));
    stats_box.add_line("Exp " + std::to_string(game_screen->player.combatant.exp));
    stats_box.add_line("G  " + std::to_string(game_screen->player.combatant.gold));
}

void MeleeScreen::draw(int frame) {
    game_screen->draw(frame);

    // Draw the background and the monster
    SDL_FRect src, dst;
    src.x = 0;
    src.y = 0;
    src.w = 150;
    src.h = 150;
    dst.x = window_w/2 - 75*(draw_w/16);
    dst.y = window_h/2 - 80*(draw_h/16);
    dst.w = 150*(draw_w/16);
    dst.h = 100*(draw_h/16);
    SDL_RenderTexture(renderer, bg_textures[bg], &src, &dst);
    if ((animate_count / 4) % 2 == 0)
        SDL_RenderTexture(renderer, monster_textures.find(enemy.name)->second, &src, &dst);

    // Recalculate box positions in case window size changed
    // Message box size is also recalculated, since x dimension is based on window width
    menu.setpos(draw_w, draw_h);
    stats_box.setpos(window_w - 7 * draw_w, draw_h);
    message_box.setpos(draw_w, window_h - 6 * draw_h);
    message_box.setsize(window_w / draw_w - 2, 5);

    message_box.draw(frame);
    stats_box.draw(frame);
    if (cur_state == melee_showing_menu)
        menu.draw(frame);
}

void MeleeScreen::init_melee(int zone) {
    // Select monster and background
    switch(zone)
    {
        case 2:
            bg = 0;
            select_enemy({"serpent", "gigarat"});
            break;
        case 3:
            bg = 0;
            select_enemy({"gigarat", "highwayman", "mage"});
            break;
        case 4:
            bg = 0;
            select_enemy({"gigarat", "highwayman", "mage"});
            break;
        case 5:
            bg = 1;
            select_enemy({"gigarat", "highwayman", "mage"});
            break;
        case 6:
            bg = 2;
            select_enemy({"gigarat", "mage"});
            break;
        case 7:
        case 8:
        case 9:
            bg = 3;
            set_enemy("serpent");
            break;
        default:
            bg = 0;
            set_enemy("serpent");
            break;
    }

    message_box.clear();
    message_box.add_line("A " + enemy.name + " appears!");
    menu.reset();
    // TODO: Maybe replace this and some other uses of random numbers with a roll_dice function
    std::uniform_int_distribution<int> ud {0, 9};
    if (ud(re) == 0) {
        message_box.add_line("The " + enemy.name + " made a surprise move!");
        cur_state = melee_monster_turn;
        combatant_action = melee_deciding;
    }
    else {
        cur_state = melee_showing_menu;
    }
    wait_frames = 10;
}

void MeleeScreen::do_menu() {
    Command cmd = get_cmd();
    switch(cmd)
    {
        case CMD_UP:
            menu--;
            wait_frames = 20;
            break;
        case CMD_DOWN:
            menu++;
            wait_frames = 20;
            break;
        case CMD_ACTION1:
        case CMD_ACTION2:
            if (menu.selection() == 0) {
                combatant_action = melee_fight;
                animate_count = 24;
                message_box.add_line(game_screen->player.combatant.name + " attacks!");
                sound->play_attack();
            }
            if (menu.selection() == 1) {
                combatant_action = melee_spell;
                // There is going to be a little more work to implement spells
                message_box.add_line(game_screen->player.combatant.name + " casts some spell!");
            }
            if (menu.selection() == 2) {
                combatant_action = melee_defend;
                message_box.add_line(game_screen->player.combatant.name + " defends!");
            }
            if (menu.selection() == 3) {
                combatant_action = melee_flee;
                message_box.add_line(game_screen->player.combatant.name + " runs away!");
            }
            set_state(melee_player_turn);
            wait_frames = 20;
            break;
        default:
            break;
    }
}

void MeleeScreen::do_player_turn() {
    switch(combatant_action)
    {
        case melee_fight:
            if (animate_count > 0)
            {
                animate_count--;
            }
            else
            {
                attack(game_screen->player.combatant, enemy);
                combatant_action = melee_done;
            }
            break;
        case melee_flee:
            if (can_flee(game_screen->player.combatant, enemy))
            {
                sound->play_steps();
                cur_screen = game_screen;
                combatant_action = melee_done;
            }
            else
            {
                message_box.add_line("But the way was blocked!");
                combatant_action = melee_done;
            }
            break;
        case melee_done:
            if (enemy.hp <= 0) {
                set_state(melee_player_won);
            }
            else {
                set_state(melee_monster_turn);
                combatant_action = melee_deciding;
            }
            wait_frames = 20;
            break;
        default:
            set_state(melee_monster_turn);
            combatant_action = melee_deciding;
            break;
    }
}

void MeleeScreen::do_monster_turn() {
    switch(combatant_action)
    {
        case melee_fight:
            message_box.add_line("The " + enemy.name + " attacks!");
            sound->play_attack();
            attack(enemy, game_screen->player.combatant);
            combatant_action = melee_done;
            break;
        case melee_flee:
            combatant_action = melee_done;
            message_box.add_line("The " + enemy.name + " flees!");
            break;
        case melee_done:
            if (game_screen->player.combatant.hp <= 0)
                set_state(melee_player_died);
            else
                set_state(melee_showing_menu);
            wait_frames = 20;
            break;
        case melee_deciding:
        default:
            combatant_action = melee_fight;
            wait_frames = 20;
            break;
    }
}

void MeleeScreen::init_monsters() {
    Combatant serpent;
    serpent.name = "serpent";
    serpent.max_hp = 5;
    serpent.strength = 3;
    serpent.resilience = 3;
    serpent.exp = 1;
    serpent.lvl = 1;
    serpent.gold = 1;
    enemies.insert({"serpent", serpent});

    Combatant gigarat;
    gigarat.name = "gigarat";
    gigarat.max_hp = 7;
    gigarat.strength = 4;
    gigarat.resilience = 3;
    gigarat.exp = 2;
    gigarat.lvl = 1;
    gigarat.gold = 2;
    enemies.insert({"gigarat", gigarat});

    Combatant highwayman;
    highwayman.name = "highwayman";
    highwayman.max_hp = 15;
    highwayman.strength = 7;
    highwayman.resilience = 5;
    highwayman.exp = 5;
    highwayman.lvl = 3;
    highwayman.gold = 3;
    enemies.insert({"highwayman", highwayman});

    Combatant mage;
    mage.name = "mage";
    mage.max_hp = 12;
    mage.strength = 9;  // This will be lowered when spellcasting ability is implemented
    mage.resilience = 3;
    mage.exp = 5;
    mage.lvl = 3;
    mage.gold = 5;
    enemies.insert({"mage", mage});
}

void MeleeScreen::set_enemy(const std::string& e) {
    enemy = enemies.find(e)->second;
    if (enemy.max_hp > 10) {
        std::uniform_int_distribution<int> ud {enemy.max_hp - (int)(0.2*enemy.max_hp), enemy.max_hp};
        enemy.hp = ud(re);
    }
    else {
        enemy.hp = enemy.max_hp;
    }

    // TODO: Decide whether to randomize mp similar to hp above
    enemy.mp = enemy.max_mp;
}

void MeleeScreen::select_enemy(std::vector<std::string> options) {
    std::uniform_int_distribution<unsigned long> ud {0, options.size() - 1};
    set_enemy(options[ud(re)]);
}

void MeleeScreen::set_state(MeleeScreen::States new_state) {
    prev_state = cur_state;
    cur_state = new_state;
}

void MeleeScreen::attack(Combatant& attacker, Combatant& defender) {
    // If defender.dexterity is at least half of attacker.dexterity, defender has a chance to dodge
    if (defender.dexterity >= attacker.dexterity / 2)
    {
        int def_advantage = defender.dexterity - attacker.dexterity;
        if (def_advantage < 0) {
            def_advantage = 0;
        }
        else {
            def_advantage = 1 + def_advantage / 10;
            // max out at 50% chance of dodging
            if (def_advantage > 5)
                def_advantage = 5;
        }
        std::uniform_int_distribution<int> ud {0, 6 + def_advantage};
        if (ud(re) >= 6) {
            message_box.add_line(attacker.name_with_article() + " missed!");
            return;
        }
    }

    // Calculate damage (if any) to defender
    // This will need to be updated to account for active effects, make it more fair/fun, etc.
    int attack = attacker.strength + attacker.weapon;
    int defense = defender.resilience + defender.armor;
    int max_dmg = attack - (defense/2);
    if (max_dmg <= 0)
        max_dmg = 1;
    int dmg = max_dmg;
    if (max_dmg > 20) {
        std::normal_distribution nd {(double)max_dmg, max_dmg/10.0};
        dmg = (int)nd(re);
        if (dmg > max_dmg) {
            dmg = 2*max_dmg - dmg;
        }
        if (dmg < 0)
            dmg = 0;
    }
    else {
        std::uniform_int_distribution<int> ud {0, max_dmg};
        dmg -= ud(re);
    }

    if (dmg == 0)
    {
        message_box.add_line("No damage!");
    }
    else
    {
        message_box.add_line(defender.name_with_article() + " took " + std::to_string(dmg) + " damage!");
        defender.hp -= dmg;
    }
}

bool MeleeScreen::can_flee(Combatant &escapee, Combatant &pursuer) {
    // Decide whether to let escapee escape...
    // This algorithm gives the escapee between 50% and 80% chance of success, depending on the difference in
    // dexterity between the combatants
    int dexterity_diff = pursuer.dexterity - escapee.dexterity;
    if (dexterity_diff < 0)
        dexterity_diff = 0;
    int pursuer_advantage = dexterity_diff / 10;
    if (pursuer_advantage > 5)
        pursuer_advantage = 5;  // Always give escapee at least 50% chance
    std::uniform_int_distribution<int> ud {0, 6 + pursuer_advantage};
    if (ud(re) > 5)
        return false;

    return true;
}

void MeleeScreen::check_level_up()
{
    int nxt_lvl = game_screen->player.combatant.lvl + 1;
    if (game_screen->player.combatant.exp >= nxt_lvl*nxt_lvl*nxt_lvl) {
        game_screen->player.level_up();
        message_box.add_line("You have reached level " + std::to_string(game_screen->player.combatant.lvl) + "!");
        message_box.add_line("Strength is now " + std::to_string(game_screen->player.combatant.strength) +
                             ", resilience is " + std::to_string(game_screen->player.combatant.resilience) + ",");
        message_box.add_line("dexterity is " + std::to_string (game_screen->player.combatant.dexterity) +
                             ", and intelligence is " + std::to_string(game_screen->player.combatant.intelligence) + ".");
        set_state(melee_wait_ack);
        prev_state = melee_over;
    }
    else
    {
        set_state(melee_over);
    }
}
