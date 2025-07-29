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
// Created by jeremiah on 6/19/25.
//

#ifndef NARTHENIAN_MENU_H
#define NARTHENIAN_MENU_H

#include <string>
#include <vector>
#include "MessageBox.h"

/**
 * @brief a menu for selecting from a list of items
 * The Menu class functions similarly to MessageBox, with a few additional features to facilitate
 * selecting an option from a list of options. The options provided to the constructor or added
 * through the add_line member function are prefixed by a space, allowing room for an arrow to the
 * left of the entry when it is the current selection. The reset member function resets the selection
 * to 0 (the first item in the list), which is useful when a particular menu is reused. The increment
 * (++) and decrement (--) operators move the current selection down and up, respectively.
 */
class Menu : public MessageBox {
private:
    int _selection = 0;
public:
    /**
     * Constructor for Menu. This class has no default constructor, so it is necessary to set the
     * parameters herein. "Pixels" in the parameters are in terms of the usually stretched pixels
     * that the game uses to place tiles, not screen pixels. The Menu should be made wide and tall
     * enough to contain its options, or else the text will overflow out of the box.
     *
     * @param x The x-coordinate of the menu's position in pixels
     * @param y The y-coordinate of the menu's position in pixels
     * @param w The width of the Menu, including the box around it, in tiles (16 pixels each)
     * @param h The height of the Menu, including the box around it, in tiles (16 pixels each)
     * @param options A list of options displayed in the menu
     */
    Menu(int x, int y, int w, int h, const std::vector<std::string>& options);
    [[nodiscard]] int selection() const;
    void reset();
    void add_line(const std::string& line) override;
    void draw(int frame) override;
    Menu& operator++();
    Menu operator++(int);
    Menu& operator--();
    Menu operator--(int);
};


#endif //NARTHENIAN_MENU_H
