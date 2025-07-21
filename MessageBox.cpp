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

#include "MessageBox.h"

#include <utility>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

SDL_Texture *font;
SDL_Texture *overlay;
extern SDL_Renderer *renderer;
extern int draw_w, draw_h;

MessageBox::MessageBox() : _x {0}, _y {0}, _w {4}, _h {4}, m {}, p {false} {

}

MessageBox::MessageBox(int x, int y, int w, int h, std::string msg, bool prompt) :
    _x {x}, _y {y}, _w {w}, _h {h}, m {std::move(msg)}, p {prompt} {

}

void MessageBox::add_line(const std::string& line) {
    m.push_back(line);
}

void MessageBox::clear() {
    m.clear();
}

void MessageBox::setpos(int new_x, int new_y) {
    _x = new_x;
    _y = new_y;
}

void MessageBox::setsize(int new_w, int new_h) {
    _w = new_w;
    _h = new_h;
}

void MessageBox::draw(int frame) {
    SDL_FRect src, dst;
    src.w = 16;
    src.h = 16;
    src.y = 0;
    dst.w = (float)draw_w;
    dst.h = (float)draw_h;

    // Draw overlay (box) -- This could probably be made less ugly
    for (int j=0; j<_h; j++)
    {
        for (int i=0; i<_w; i++)
        {
            dst.x = (float)(_x + (draw_w*i));
            dst.y = (float)(_y + (draw_h*j));
            if (i==0 && j==0)   // top left corner
                src.x = 0;
            else if (i==_w-1 && j==0) // top right corner
                src.x = 32;
            else if (i==0 && j==_h-1) // bottom left corner
                src.x = 96;
            else if (i==_w-1 && j==_h-1) // bottom right corner
                src.x = 128;
            else if (i==_w-2 && j==_h-1 && p)  // near bottom right corner, and prompting user to press key/button
                src.x = (float)(144+16*frame);  // Animates an arrow
            else if (i==0)  // left side
                src.x = 48;
            else if (i==_w-1) // right side
                src.x = 80;
            else if (j==0) // top
                src.x = 16;
            else if (j==_h-1) // bottom
                src.x = 112;
            else    // middle portion
                src.x = 64;
            SDL_RenderTexture(renderer, overlay, &src, &dst);
        }
    }

    // Draw message text
    unsigned long lines = (int)(1.34*(_h-2));
    if (lines > m.size())
        lines = m.size();

    for (unsigned long l=m.size() - lines; l<m.size(); l++)
    {
        // Yes, that's some ugly calculating and casting
        // OOPS: I was displaying the lines in reverse order
        //draw_str(_x + draw_w, (_y + draw_h + (int) ((float) (m.size() - (l + 1)) * ((float) draw_h / 1.333f))), m[l]);
        draw_str(_x + draw_w, (_y + draw_h + (int) ((float)(l-(m.size()-lines)) * ((float) draw_h / 1.333f))), m[l]);
    }
}

void MessageBox::draw_str(int x, int y, const std::string& str) {
    SDL_FRect src, dst;
    src.y = 0;
    src.w = 8;
    src.h = 12;
    dst.x = (float)x;
    dst.y = (float)y;
    dst.w = (float)draw_w/2;
    dst.h = (float)draw_h/1.333f;
    for (auto c : str)
    {
        src.x = 80; // default to space
        if (c >= '0' && c <= '9')
            src.x = (float)((c-'0')*8);
        if (c >= 'A' && c <= 'Z')
            src.x = (float)(120+(c-'A')*8);
        if (c >= 'a' && c <= 'z')
            src.x = (float)(328+(c-'a')*8);
        if (c == '.') src.x = 88;
        if (c == ',') src.x = 96;
        if (c == '?') src.x = 104;
        if (c == '!') src.x = 112;
        SDL_RenderTexture(renderer, font, &src, &dst);
        dst.x += (float)draw_w/2;
    }
}

void MessageBox::init_font() {
    font = IMG_LoadTexture(renderer, "../assets/font.png");
    overlay = IMG_LoadTexture(renderer, "../assets/overlay.png");
    SDL_SetTextureScaleMode(font, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(overlay, SDL_SCALEMODE_NEAREST);
}

void MessageBox::destroy_font() {
    SDL_DestroyTexture(font);
    SDL_DestroyTexture(overlay);
}
