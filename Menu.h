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

class Menu : public MessageBox {
private:
    int _selection = 0;
public:
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
