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
// Created by jeremiah on 7/13/25.
//

#ifndef NARTHENIAN_SOUND_H
#define NARTHENIAN_SOUND_H

#include <SDL3/SDL.h>

class Sound {
public:
    Sound();
    ~Sound();
    void play_bump();
    void play_steps();
    void play_attack();
private:
    bool audio_on = true;
    bool music_on = true;
    SDL_AudioStream *sfx_stream;
    Uint8 *bump_data;
    Uint32 bump_len;
    Uint8 *step_data;
    Uint32 step_len;
    Uint8 *attack_data;
    Uint32 attack_len;
    void play_sound(Uint8* data, Uint32 len);
};


#endif //NARTHENIAN_SOUND_H
