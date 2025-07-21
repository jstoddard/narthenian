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

#include "Menu.h"

#include <SDL3/SDL.h>

extern SDL_Texture *font;
extern SDL_Renderer *renderer;
extern int draw_w, draw_h;

Menu::Menu(int x, int y, int w, int h, const std::vector<std::string>& options) {
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    for (const auto& opt : options)
        m.push_back(" " + opt);
}

void Menu::add_line(const std::string& line) {
    m.push_back(" " + line);
}

void Menu::draw(int frame) {
    SDL_FRect src, dst;

    // Draw message box as normal
    MessageBox::draw(frame);

    // Add indicator for current selection
    src.x = 536;
    src.y = 0;
    src.w = 8;
    src.h = 12;
    dst.x = (float)(_x+draw_w);
    dst.y = (float)(_y+draw_h)+((float)(_selection)*((float)draw_h/1.333f));
    dst.w = (float)draw_w/2.0f;
    dst.h = (float)draw_h/1.333f;
    SDL_RenderTexture(renderer, font, &src, &dst);
}

int Menu::selection() const {
    return _selection;
}

Menu &Menu::operator++() {
    if (_selection+1 < m.size())
        _selection++;
    return *this;
}

Menu Menu::operator++(int) {
    Menu old = *this;
    operator++();
    return old;
}

Menu &Menu::operator--() {
    if (_selection > 0)
        _selection--;
    return *this;
}

Menu Menu::operator--(int) {
    Menu old = *this;
    operator--();
    return old;
}

void Menu::reset() {
    _selection = 0;
}
