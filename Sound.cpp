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

#include "Sound.h"
#include "common.h"

Sound::Sound() {
    SDL_AudioSpec spec;
    // Reusing spec because all the wav files should be the same format
    SDL_LoadWAV((asset_dir / "bump.wav").c_str(), &spec, &bump_data, &bump_len);
    SDL_LoadWAV((asset_dir / "step.wav").c_str(), &spec, &step_data, &step_len);
    SDL_LoadWAV((asset_dir / "attack.wav").c_str(), &spec, &attack_data, &attack_len);
    sfx_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(sfx_stream);
}

Sound::~Sound() {
    SDL_DestroyAudioStream(sfx_stream);
    SDL_free(bump_data);
    SDL_free(step_data);
    SDL_free(attack_data);
}

void Sound::play_bump() {
    play_sound(bump_data, bump_len);
}

void Sound::play_steps() {
    play_sound(step_data, step_len);
}

void Sound::play_attack() {
    play_sound(attack_data, attack_len);
}

void Sound::play_sound(Uint8 *data, Uint32 len) {
    if (!audio_on)
        return;
    SDL_ClearAudioStream(sfx_stream);
    SDL_PutAudioStreamData(sfx_stream, data, (int)len);
}
