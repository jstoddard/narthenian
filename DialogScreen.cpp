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
// Created by jeremiah on 7/14/25.
//

#include "DialogScreen.h"

extern int window_w, window_h, draw_w, draw_h;
extern Screen *cur_screen;
extern GameScreen *game_screen;

DialogScreen::DialogScreen()
:
    prompt_menu(draw_w, draw_h, 12, 10, {}),
    message_box(draw_w, window_h - 6 * draw_h, window_w / draw_w - 2, 5, "", true) {

}

void DialogScreen::iterate() {
    if (!cur_npc)
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
        case ds_load_dialog:
            talk("");
            break;
        case ds_prompt:
            break;
        case ds_wait_ack: {
            Command cmd = get_cmd();
            if (cmd == CMD_ACTION1 || cmd == CMD_ACTION2) {
                message_box.prompt() = false;
                set_state(ds_done);
            }
            break;
        }
        case ds_done:
            cur_screen = game_screen;
            break;
    }
}

void DialogScreen::draw(int frame) {
    game_screen->draw(frame);

    message_box.setpos(draw_w, window_h - 6 * draw_h);
    message_box.setsize(window_w / draw_w - 2, 5);
    message_box.draw(frame);
}

void DialogScreen::set_npc(NPC *new_npc) {
    cur_npc = new_npc;
    message_box.clear();
    set_state(ds_load_dialog);
}

void DialogScreen::use_dialog_entry(const NPC::DialogEntry& de) {
    // Display the dialog entry on screen, give user items and update quests as appropriate
    for (const auto& line : de.lines) {
        message_box.add_line(line);
    }
    set_state(ds_wait_ack);
    wait_frames = 20; // Give player a chance to take his/her hand off the action button
}

void DialogScreen::talk(const std::string& prompt) {
    // Search through dialog entries to find an available one
    // We want to show the most specific prompt available for the current game state, so when the quest system is a
    // little more complete, we'll go through the quest related dialogs first. For now, start with prompt, assuming
    // it is not blank, and if nothing is found, move along to generic dialog entries
    if (!prompt.empty()) {
        for (const auto& de : cur_npc->dialog_entries) {
            if (de.condition == NPC::DialogCondition::dc_prompt && de.prompt == prompt) {
                use_dialog_entry(de);
                return;
            }
        }
    }

    // If no dialog entry has been found so far, let's try to find a generic one
    for (const auto& de: cur_npc->dialog_entries)
    {
        if (de.condition == NPC::DialogCondition::dc_none) {
            use_dialog_entry(de);
            return;
        }
    }

    // No dialog entry to use; display a generic message and end the dialog.
    message_box.add_line("Hello!");
    set_state(ds_wait_ack);
    wait_frames = 20; // Give player a chance to take his/her hand off the action button
}

void DialogScreen::set_state(DialogScreen::DialogScreenState new_state) {
    state = new_state;
}
