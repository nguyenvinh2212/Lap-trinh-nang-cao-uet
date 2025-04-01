
#include "main.h"
Stage stage;
App app;
Menu menu;
TextRenderer* textManager = new TextRenderer("font/EnterCommand.ttf", 30);
HighScoreManager* HighScore_Manager = new HighScoreManager("Highscore.txt");
bool gameState = false;
bool pauseGame = false;
int pauseChoice = 0;
Uint32 deltaTime;
int main(int argc, char* argv[])
{
    srand(time(0));
    initSDL();
    initgame();
    initStage();
    menu.init();

    Uint32 lastTime = SDL_GetTicks();
    while (app.running)
    {
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        doInput();

        // tăng giảm âm lượng game;
        Mix_Volume(-1, MIX_MAX_VOLUME * soundVolume / 100);
        Mix_VolumeMusic(MIX_MAX_VOLUME * musicVolume / 100);

        if (gameState) // game dùng
        {
            if (pauseGame) // khi pause game hiển thị continue với back to menu
            {
                if (app.keyboard[SDL_SCANCODE_LEFT])
                {
                    pauseChoice = 0;
                    app.keyboard[SDL_SCANCODE_LEFT] = 0;
                }
                if (app.keyboard[SDL_SCANCODE_RIGHT])
                {
                    pauseChoice = 1;
                    app.keyboard[SDL_SCANCODE_RIGHT] = 0;
                }
                if (app.keyboard[SDL_SCANCODE_RETURN])
                {
                    if (pauseChoice == 0) {
                        pauseGame = false;
                    }
                    else
                    {
                        gameState = false;
                        pauseGame = false;
                        stage.reset();
                        stage.initPlayer();
                        menu.init();
                    }
                    app.keyboard[SDL_SCANCODE_RETURN] = 0;
                }
                // Nếu nhấn ESC tiếp thì chơi tiếp
                if (app.keyboard[SDL_SCANCODE_ESCAPE])
                {
                    pauseGame = false;
                    app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
                }
                // dừng logic
                prepareScene();
                stage.draw();
                textManager->setFontSize(40);
                SDL_Color continueColor = (pauseChoice == 0) ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 255, 255, 255};
                SDL_Color menuColor     = (pauseChoice == 1) ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 255, 255, 255};

                textManager->drawText("PAUSED", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 255, 255, 255, TEXT_CENTER);
                textManager->drawText("Continue", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, continueColor.r, continueColor.g, continueColor.b, TEXT_CENTER);
                textManager->drawText("Back to Menu", SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2, menuColor.r, menuColor.g, menuColor.b, TEXT_CENTER);
                presentScene();
            }
            else  // Nếu không tạm dừng
            {
                if (app.keyboard[SDL_SCANCODE_ESCAPE])
                {
                    pauseGame = true;
                    pauseChoice = 0;
                    app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
                }
                // Cập nhật logic stage và vẽ
                prepareScene();
                stage.logic();
                stage.draw();
                presentScene();
           }
        }
        else
        {
            menu.update();
            prepareScene();
            menu.draw();
            presentScene();
        }

        // Giới hạn FPS
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < 1000 / 60)
            SDL_Delay(1000 / 60 - frameTime);
    }
    close();
    return 0;
}
