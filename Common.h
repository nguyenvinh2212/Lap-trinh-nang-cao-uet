
#ifndef Common_h
#define Common_h

#include<iostream>
#include<math.h>
#include<stdlib.h>

#include<cstring>
#include <string>
#include<vector>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include"def.h"
#include "Game_object.h"

struct App  // đại diện cho ứng dụng game
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int keyboard[MAX_KEYBOARD_KEYS];
    bool running = true;
};
SDL_Texture *loadTexture(const char *filename);
extern App app;
#endif
