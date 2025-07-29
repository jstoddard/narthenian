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

#include "Innkeeper.h"

Innkeeper::Innkeeper(int _sprite, int _x, int _y, Direction _dir, int _check_x, int _check_y, int _price,
                     std::vector<std::string> _greeting) :
                     sprite(_sprite), x(_x), y(_y), dir(_dir),
                     check_x(_check_x), check_y(_check_y),
                     price(_price), greeting(_greeting) {

}
