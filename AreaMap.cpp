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
// Created by jeremiah on 6/11/25.
//

#include <fstream>
#include <stdexcept>
#include <SDL3_image/SDL_image.h>
#include "AreaMap.h"

extern int window_h, window_w;
extern int draw_h, draw_w;
extern SDL_Renderer *renderer;

AreaMap::AreaMap(const std::string &tileset_fn, const std::string &map_fn) {
    // Open fn and read map
    unsigned char low, high;
    int i;

    /* Load tileset -- doing this separately for every map seems wasteful
     * when we only use a single tileset, but this way it will be easy to use
     * different tilesets for cities, dungeons, etc. if we decide to do so
     * later. */
    tileset = IMG_LoadTexture(renderer, tileset_fn.c_str());
    if (!tileset)
    {
        SDL_Log("Couldn't load tileset: %s\n", SDL_GetError());
        throw std::runtime_error("Could not load tileset!");
    }
    /* Ideally, SDL_SCALEMODE_PIXELART, but this option is not in the 3.2.16
     * release of SDL3 (latest release as of 6/10/2025); I wish the SDL docs
     * were more clear about this. */
    SDL_SetTextureScaleMode(tileset, SDL_SCALEMODE_NEAREST);

    std::ifstream mapfile(map_fn, std::ios::binary);
    // Get width and height of map, stored as 16-bit integers in little endian
    mapfile.read(reinterpret_cast<char *>(&low), 1);
    mapfile.read(reinterpret_cast<char *>(&high), 1);
    w = 256 * high + low;
    mapfile.read(reinterpret_cast<char *>(&low), 1);
    mapfile.read(reinterpret_cast<char *>(&high), 1);
    h = 256 * high + low;

    // allocate memory for map
    map.resize(w*h);
    zones.resize(w*h);

    // read tiles and then zones
    mapfile.read(reinterpret_cast<char *>(map.data()), w * h);
    mapfile.read(reinterpret_cast<char *>(zones.data()), w * h);
}

AreaMap::~AreaMap() {
    SDL_DestroyTexture(tileset);
}

int AreaMap::zone(int x, int y) {
    if (x >= 0 && x < w && y >= 0 && y < h)
        return zones[w*y+x];
    else
        return 0;
}

void AreaMap::draw(int x_start, int y_start, int frame) {
    SDL_FRect src, dst;
    int x;
    int y;
    int tile;

    src.w = 16;
    src.h = 16;
    dst.w = (float)draw_w;
    dst.h = (float)draw_h;

    // +1 to 16*window_h/draw_h to avoid black bar on bottom of screen after some window resizes
    for (y = -(y_start % 16); y < 16*(window_h/draw_h) + 1; y += 16)
    {
        // had 16*(window_w/draw_w), but left black bar on right side of window sometimes after resize
        for (x = -(x_start % 16); x < 17*(window_w/draw_w); x += 16)
        {
            int tile_y = (y_start+y)/16;
            int tile_x = (x_start+x)/16;
            // There are some attempted out-of-bounds accesses here on the bottom of the map, probably because
            // we are drawing an extra row just to cover window resizes above.
            // TODO: Check back after moving to fullscreen and no longer drawing an extra row and column to
            //  see if we still need this check. If so, there may be a bug we should fix.
            if (tile_y < 0 || tile_y >= h || tile_x < 0 || tile_x >= w)
                tile = 0;
            else
                tile = map[w*tile_y+tile_x];
            if (tile >= 128)
                tile = (tile - 128) + frame;
            src.x = (float)((tile % 16) * 16);
            src.y = (float)((tile / 16) * 16);
            dst.x = (float)x*draw_w/16;
            dst.y = (float)y*draw_h/16;
            SDL_RenderTexture(renderer, tileset, &src, &dst);
        }
    }
}
