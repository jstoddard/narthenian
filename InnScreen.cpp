//
// Created by jeremiah on 7/27/25.
//

#include "InnScreen.h"
#include "GameScreen.h"

extern int window_w, window_h, draw_w, draw_h;
extern Screen *cur_screen;
extern GameScreen *game_screen;
extern SDL_Renderer *renderer;

InnScreen::InnScreen() :
        menu(draw_w, window_h - 10 * draw_h, 5, 4, {"Yes", "No" }),
        message_box(draw_w, window_h - 6 * draw_h, window_w / draw_w - 2, 5, "", false) {

}

void InnScreen::reset_inn(Innkeeper *new_innkeeper) {
    innkeeper = new_innkeeper;
    message_box.clear();
    for (auto& g : innkeeper->greeting) {
        message_box.add_line(g);
    }
    menu.reset();
    set_state(inn_deciding);
}

void InnScreen::iterate(void) {
    if (!innkeeper)
    {
        cur_screen = game_screen;
        return;
    }
    if (wait_frames > 0)
    {
        wait_frames--;
        return;
    }
    switch(state) {
        case inn_none:
            cur_screen = game_screen;
            break;
        case inn_deciding:
            do_menu();
            break;
        case inn_sleeping1: {
            if (effect_frame < 30) {
                effect_frame++;
            }
            else {
                game_screen->player.combatant.hp = game_screen->player.combatant.max_hp;
                game_screen->player.combatant.mp = game_screen->player.combatant.max_mp;
                set_state(inn_sleeping2);
                wait_frames = 30;
            }
            break;
        }
        case inn_sleeping2: {
            if (effect_frame > 0) {
                effect_frame--;
            }
            else {
                set_state(inn_finished);
            }
            break;
        }
        case inn_nsf:
            message_box.add_line("Sorry, you do not have enough money!");
            message_box.prompt() = true;
            set_state(inn_wait_ack);
            break;
        case inn_wait_ack: {
            Command cmd = get_cmd();
            if (cmd == CMD_ACTION1 || cmd == CMD_ACTION2) {
                message_box.prompt() = false;
                set_state(inn_none);
            }
            break;
        }
        case inn_finished:
            message_box.add_line("Thank you for your stay!");
            message_box.prompt() = true;
            set_state(inn_wait_ack);
            break;
    }

    stats_box.clear();
    stats_box.add_line("HP " + std::to_string(game_screen->player.combatant.hp));
    stats_box.add_line("MP " + std::to_string(game_screen->player.combatant.mp));
    stats_box.add_line("Lvl " + std::to_string(game_screen->player.combatant.lvl));
    stats_box.add_line("Exp " + std::to_string(game_screen->player.combatant.exp));
    stats_box.add_line("G  " + std::to_string(game_screen->player.combatant.gold));
}

void InnScreen::draw(int frame) {
    game_screen->draw(frame);

    stats_box.setpos(window_w - 7 * draw_w, draw_h);
    stats_box.setsize(6, 6);
    stats_box.draw(frame);

    message_box.setpos(draw_w, window_h - 6 * draw_h);
    message_box.setsize(window_w / draw_w - 2, 5);
    message_box.draw(frame);

    if (state == inn_deciding) {
        menu.setpos(draw_w, window_h - 10 * draw_h);
        menu.draw(frame);
    }

    if (state == inn_sleeping1 || state == inn_sleeping2) {
        sleep_effect();
    }
}

void InnScreen::set_state(InnScreen::InnScreenState new_state) {
    state = new_state;
}

void InnScreen::sleep_effect(void) {
    Uint8 alpha = (effect_frame / 30.0) * 255;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_RenderFillRect(renderer, NULL);
}

void InnScreen::do_menu(void) {
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
                if (game_screen->player.combatant.gold >= innkeeper->price) {
                    game_screen->player.combatant.gold -= innkeeper->price;
                    set_state(inn_sleeping1);
                }
                else {
                    set_state(inn_nsf);
                }
            }
            if (menu.selection() == 1) {
                set_state(inn_none);
            }
        default:
            break;
    }
}
