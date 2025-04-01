
#include "Background.h"

static int background_Y;
static Star stars[MAX_STARS];
static SDL_Texture *background;

void initBackground()
{
    background = loadTexture( "Image/Backgound_level_1.png");
    background_Y = 0;
    // set rand() thuộc tính của stars
    for(int i = 0; i < MAX_STARS; i++){
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = rand() % (MAX_SPEED_STARS - 1) +1;
    }
}
void doBackground() // cap nhat vi tri background và stars
{
    background_Y --;
    if( --background_Y < -SCREEN_HEIGHT)
        background_Y = 0;
    for(int i = 0; i < MAX_STARS; i++){
        stars[i].y += stars[i].speed;
        if(stars[i].y > SCREEN_HEIGHT){
            stars[i].y -=SCREEN_HEIGHT;
        }
    }
}
// vẽ background ra màn hình
void drawBackground()
{
    SDL_Rect dest;
    for(int i = background_Y ; i < SCREEN_HEIGHT; i+= SCREEN_HEIGHT)
    {
        dest.x = 0;
        dest.y = i;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;
        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
    	for (int i = 0; i < MAX_STARS; i++)
	{
		int c = 85 * stars[i].speed;
		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);
		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x , stars[i].y +3 );
	}
}
