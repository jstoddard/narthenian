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

#ifndef NARTHENIAN_INNKEEPER_H
#define NARTHENIAN_INNKEEPER_H

#include <string>
#include <vector>
#include "common.h"

/**
 * @brief An innkeeper
 * This class represents an innkeeper. Unlike an NPC from the NPC class, this character does not
 * have complicated dialog abilities. Moreover, the location the game looks for an Innkeeper does
 * not generally match the Innkeeper's location, since the player will generally interact with
 * the Innkeeper across a counter.
 */
class Innkeeper {
public:
    /**
     * Constructor for Innkeeper. Greeting is suggested to be something like, "Welcome to
     * <town>'s inn. Price is 5 gold per night. Would you like to spend the night here?"
     * It should be put into a vector of up to four strings with no more than about 36
     * characters each.
     *
     * @param _sprite id of the sprite to be used, generally the same as the merchant sprite
     * @param _x x coordinate of location on map
     * @param _y y coordinate of location on map
     * @param _dir the direction that the innkeeper is facing
     * @param _check_x x coordinate of counter, or same as x if no counter
     * @param _check_y y coordinate of counter, or same as y if no counter
     * @param _price price charged to the player for using this inkeeper's inn
     * @param _greeting greeting to player
     */
    Innkeeper(int _sprite, int _x, int _y, Direction _dir, int _check_x, int _check_y, int _price, std::vector<std::string> _greeting);
    int sprite;
    int x;
    int y;
    Direction dir;
    int check_x;
    int check_y;
    int price;
    std::vector<std::string> greeting;
};


#endif //NARTHENIAN_INNKEEPER_H
