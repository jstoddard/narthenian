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
// Created by jeremiah on 6/14/25.
//
#include "Screen.h"

Screen::Command Screen::get_cmd() {
    static const bool *keys = SDL_GetKeyboardState(nullptr);
    static bool action1_prev_state = false;
    static bool action2_prev_state = false;
    // TODO: Gamepad support, maybe user-editable controls
    Command cmd = CMD_NONE;
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W])
        cmd = CMD_UP;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
        cmd = CMD_DOWN;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
        cmd = CMD_LEFT;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
        cmd = CMD_RIGHT;
    if (!keys[SDL_SCANCODE_RETURN]) {
        action1_prev_state = false;
    }
    else if (action1_prev_state == false) {
        cmd = CMD_ACTION1;
        action1_prev_state = true;
    }
    if (!keys[SDL_SCANCODE_SPACE]) {
        action2_prev_state = false;
    }
    else if (action2_prev_state == false) {
        cmd = CMD_ACTION2;
        action2_prev_state = true;
    }
    return cmd;
}
