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

#ifndef NARTHENIAN_SCREEN_H
#define NARTHENIAN_SCREEN_H

#include <SDL3/SDL.h>
/**
 * @brief A game screen
 * This is the base class for the different interfaces, such as the main menu, credits, or game screen, a user
 * may be presented with. Derived classes should override the iterate() and draw() functions, which are called
 * in order once per frame while the screen is active. The function get_cmd() may be called, usually in iterate(),
 * to check for user input.
 */
class Screen {
protected:
    enum Command
    {
        CMD_NONE,
        CMD_UP,
        CMD_DOWN,
        CMD_LEFT,
        CMD_RIGHT,
        CMD_ACTION1,
        CMD_ACTION2
    };
    /**
     * Returns an action to do based on the state of the keyboard or controller.
     * We don't worry about multiple key presses at once since this is a simple
     * RPG; only one command at a time is considered.
     *
     * @return user commend
     */
    static Command get_cmd();
public:
    virtual void iterate() = 0;
    virtual void draw(int frame) = 0;
};

#endif //NARTHENIAN_SCREEN_H
