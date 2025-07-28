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

#ifndef NARTHENIAN_AREAMAP_H
#define NARTHENIAN_AREAMAP_H

#include <string>
#include <vector>
#include <list>
#include <SDL3/SDL.h>
#include "NPC.h"
#include "Innkeeper.h"

/**
 * @brief A map of an area the user might be in
 * An AreaMap contains the tile data, portals (entrances to other maps), and other
 * relevant data for a map such as the overworld, a town, a dungeon, etc.
 */
class AreaMap {
private:
    int w;
    int h;
    std::vector<unsigned char> map;
    std::vector<unsigned char> zones;
    SDL_Texture *tileset;
public:
    AreaMap(const std::string &tileset_fn, const std::string &map_fn);
    ~AreaMap();

    /**
     * @brief An entry point into another map
     * A portal indicates a position (x, y) that, when stepped on, transports the user to another map. This is
     * used, e.g., for entry into a town or dungeon, or for returning to the overworld from a town or dungeon.
     * dst_map identifies which map the user will go to when stepping on the Portal. down_x, down_y, etc. indicate the
     * place on the destination map the user will be at: There are separate sets of destination points to allow the
     * user to be placed depending on which side the user entered on, such as the left or right side of town. In
     * many cases, all sets of destination points will be the same.
     */
    struct Portal {
        std::string dst_map;
        int x, y;
        int down_x, down_y;
        int left_x, left_y;
        int right_x, right_y;
        int up_x, up_y;
    };
    std::vector<Portal> portals {};
    std::vector<NPC> npcs {};
    std::vector<Innkeeper> innkeepers {};

    /**
     * @brief Returns the zone for the given tile
     * Returns the zone for the tile at the given location. Zone 0 means that
     * the tile is a barrier that the player cannot walk onto, such as a
     * mountain or water. The player can walk on to zones > 0. The zone
     * determines what enemies the player may encounter on this portion of
     * the map, with 1 being peaceful (no enemies).
     *
     * @param x X-coordinate of location to check (in tiles, not pixels!)
     * @param y Y-coordinate of location
     * @return The Zone corresponding to the map location
     */
    int zone(int x, int y);
    /**
     * @brief Renders the map
     * Renders the map to the SDL renderer for display on screen, beginning
     * at x_start, y_start (in pixels) from the top left of the map and
     * starting at the top left of the screen or window.
     *
     * @param x_start starting location in pixels from left edge of map
     * @param y_start starting location in pixels from top of map
     * @param frame a frame between 0 and 3 to draw for animated tiles
     */
    void draw(int x_start, int y_start, int frame);
    [[nodiscard]] int width() const {return w;}
    [[nodiscard]] int height() const {return h;}
};


#endif //NARTHENIAN_AREAMAP_H
