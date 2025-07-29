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
// Created by jeremiah on 7/27/25.
//

#ifndef NARTHENIAN_INNSCREEN_H
#define NARTHENIAN_INNSCREEN_H


#include "Screen.h"
#include "MessageBox.h"
#include "Menu.h"
#include "Innkeeper.h"

class InnScreen : public Screen {
private:
    MessageBox message_box;
    MessageBox stats_box;
    Menu menu;
    Innkeeper *innkeeper = nullptr;
    enum InnScreenState { inn_none, inn_deciding, inn_sleeping1, inn_sleeping2, inn_nsf, inn_wait_ack, inn_finished };
    InnScreenState state = inn_none;
    int effect_frame;
    int wait_frames;
    void set_state(InnScreenState new_state);
    void sleep_effect(void);
    void do_menu(void);

public:
    InnScreen();
    void reset_inn(Innkeeper *new_innkeeper);
    void iterate(void) override;
    void draw(int frame) override;
};


#endif //NARTHENIAN_INNSCREEN_H
