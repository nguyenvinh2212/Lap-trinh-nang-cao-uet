
#ifndef Stage_h
#define Stage_h
#include "Common.h"
#include "Animation.h"
#include "Graphic.h"
#include "PlayerBulletUpgrade.h"
extern bool gameState;
float planet_y = 100;
extern Stage stage;
extern App app;
extern TextRenderer* textManager;
SoundManager sound;
extern void doBackground();
extern void drawBackground();
extern void blit(SDL_Texture* texture, int x, int y);
extern void blitRect(SDL_Texture *texture, SDL_Rect *src, float x, float y);
extern drawText(int x, int y, int r, int g, int b, int align, const std::string &text, int size);
extern calcSlope(int x1,int y1, int x2, int y2, float& dx, float& dy);
extern collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
#endif // Stage_h
