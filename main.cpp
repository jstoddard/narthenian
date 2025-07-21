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

#define SDL_MAIN_USE_CALLBACKS 1

#include <random>
#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "common.h"
#include "Screen.h"
#include "GameScreen.h"
#include "MeleeScreen.h"
#include "DialogScreen.h"
#include "MessageBox.h"
#include "Sound.h"

#define DISPLAY_WIDTH   384
#define DISPLAY_HEIGHT  240
// We want a 16:10 aspect ratio, but will settle for anything from a little
// less than 16:10 to a little more than 16:9
#define MIN_ASPECT_RATIO 1.58
#define MAX_ASPECT_RATIO 1.78

/* Directories for assets and saved games */
std::filesystem::path asset_dir;
std::filesystem::path save_dir;

/* SDL Stuff */
int window_w, window_h; /* Track the height and width of the window/screen in pixels */
int draw_w, draw_h;     /* Height and width to draw each tile/sprite */
static SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

/* Random number generator */
std::mt19937 re;

/* Game objects */
Sound *sound;
GameScreen *game_screen;
MeleeScreen *melee_screen;
DialogScreen *dialog_screen;
Screen *cur_screen;

void requestAspectRatio(void *);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Set up paths
    auto p = SDL_GetBasePath();
    if (!p) {
        SDL_Log("SDL_GetBasePath failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    save_dir = p;
    asset_dir = p;
    asset_dir += "../assets/";
    asset_dir = asset_dir.lexically_normal();
    SDL_free((void *) p);
    SDL_Log("asset_dir: %s", asset_dir.c_str());
    p = SDL_GetPrefPath("avfig", "narthenian");
    if (!p) {
        // Interestingly, after this, SDL_GetBasePath() may no longer work.
        // (This is why save_dir was previously set above.)
        SDL_Log("SDL_GetPrefPath failed: %s", SDL_GetError());
        SDL_Log("Warning: Using base path, so saving may not work!");
    }
    else {
        save_dir = p;
        SDL_free((void *) p);
    }
    save_dir = save_dir.lexically_normal();
    SDL_Log("save_dir: %s", save_dir.c_str());

    window_w = DISPLAY_WIDTH * 2;
    window_h = DISPLAY_HEIGHT * 2;
    draw_w = draw_h = window_h / 15;
    if (!SDL_CreateWindowAndRenderer("The Narthenian Conspiracy", window_w, window_h, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    //SDL_SetRenderLogicalPresentation(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);

    // Seed the random number generator
    std::random_device rd;
    re.seed(rd());

    // Set up sound device and load sound effects and music
    sound = new Sound();

    MessageBox::init_font();

    game_screen = new GameScreen();
    melee_screen = new MeleeScreen();
    dialog_screen = new DialogScreen();
    cur_screen = game_screen;

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    /* Check input, do any other tasks necessary this frame */
    cur_screen->iterate();

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    /* Calculate frame for animations */
    Uint64 time_elapsed = SDL_GetTicks();
    int frame = (int)((time_elapsed / 250) % 4);

    cur_screen->draw(frame);  // Draw whatever we need to draw in this part of the game
    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    SDL_Delay(10);  // Try not to get carried away with CPU use
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, key presses, etc.) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
        case SDL_EVENT_WINDOW_RESIZED:
            if (window_w != event->window.data1 || window_h != event->window.data2)
            {
                window_w = event->window.data1;
                window_h = event->window.data2;
                draw_w = draw_h = window_h / 15;    /* Scale tiles so that 15 can be displayed vertically */
                float a = (float)window_w/(float)window_h; // Aspect ratio
                if (a < MIN_ASPECT_RATIO || a > MAX_ASPECT_RATIO)
                    SDL_RunOnMainThread(requestAspectRatio, nullptr, false);
            }
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE)
            {
                return SDL_APP_SUCCESS;
            }
        default:
            break;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    delete game_screen;
    delete melee_screen;
    delete dialog_screen;
    MessageBox::destroy_font();
    delete sound;
}

void requestAspectRatio(void *userdata)
{
    SDL_SetWindowAspectRatio(window, MIN_ASPECT_RATIO, MAX_ASPECT_RATIO);
}
