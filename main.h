#ifndef main_h
#define main_h

#include "Common.h"
#include "Menu.h"
#include "Graphic.h"
#include "Highscore.h"
#include <time.h>
extern void initgame();
extern void initSDL();
extern void initStage();
extern void prepareScene();
extern void presentScene();
extern void close();
extern doInput();
extern int soundVolume;
extern int musicVolume;
#endif // main_h
