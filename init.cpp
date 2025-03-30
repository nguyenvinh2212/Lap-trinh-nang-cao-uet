
#include "init.h"
#include "Graphic.h"
extern SoundManager sound;
void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
}
void initSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(app.window == NULL) logErrorAndExit("Create Window", SDL_GetError());
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit( "SDL_image error:", IMG_GetError());
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (app.renderer == nullptr) logErrorAndExit("Create Renderer", SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(app.renderer,SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(app.renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR );
}
void initgame()
{
    initBackground();
    sound.loadMusic("sound/Mercury.ogg");
    sound.playMusic(1);
}
