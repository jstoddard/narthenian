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

#ifndef NARTHENIAN_DIALOGSCREEN_H
#define NARTHENIAN_DIALOGSCREEN_H

#include <string>
#include "GameScreen.h"
#include "MessageBox.h"
#include "Menu.h"
#include "NPC.h"

class DialogScreen : public Screen {
private:
    MessageBox message_box;
    Menu prompt_menu;
    std::string current_prompt;
    int wait_frames = 0;
    NPC *cur_npc = nullptr;
    enum DialogScreenState { ds_load_dialog, ds_prompt, ds_wait_ack, ds_done };
    DialogScreenState state = ds_done;
    void use_dialog_entry(const NPC::DialogEntry& de);
    void set_state(DialogScreenState new_state);

public:
    DialogScreen();
    void iterate() override;
    void draw(int frame) override;
    void set_npc(NPC *new_npc);
    void talk(const std::string& prompt);
};


#endif //NARTHENIAN_DIALOGSCREEN_H
