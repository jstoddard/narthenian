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
// Created by jeremiah on 6/15/25.
//

#ifndef NARTHENIAN_MESSAGEBOX_H
#define NARTHENIAN_MESSAGEBOX_H

#include <string>
#include <vector>

/**
 * @brief A message box for displaying text on screen
 * The message box class provides a bordered box and text to show on screen.
 * The add_line method adds a line of text to the message box's method buffer.
 * Call the draw method each frame to display the message box and the last lines of the buffer, however many fit in
 * the box. (Note that no word wrapping is provided, and long lines may draw outside of the box.)
 * The prompt method gets or sets a boolean that determines whether a prompt symbol is shown at the bottom of the
 * box to indicate that the game is waiting for the user's acknowledgement to continue.
 */
class MessageBox {
protected:
    int _x, _y, _w, _h;
    bool p;
    std::vector<std::string> m;
public:
    MessageBox();
    MessageBox(int x, int y, int w, int h, std::string msg, bool prompt);
    virtual void add_line(const std::string& line);
    void clear();

    virtual void draw(int frame);
    void setpos(int new_x, int new_y);
    void setsize(int new_w, int new_h);
    bool& prompt() { return p; }
    [[nodiscard]] const bool& prompt() const { return p; }
    static void init_font();
    static void draw_str(int x, int y, const std::string& str);
    static void destroy_font();
};


#endif //NARTHENIAN_MESSAGEBOX_H
