#ifndef MENU_H
#define MENU_H

#include "Common.h"

struct MenuItem {
    std::string label;
    int x, y;
    SDL_Texture* normal;
    SDL_Texture* selected;
    void (*action)();
};

class Menu {
public:
    Menu();
    ~Menu(){};
    void init();
    void update();
    void draw();
    void switchToOptions();
    void switchToMain();
    void switchToControls();
    void switchToHighScores();
private:
    int activeIndex;
    bool active;
    SDL_Texture* title;
    bool inOptionsMenu;
    bool inControlsMenu;
    bool inHighScoresMenu;

    std::vector<MenuItem> items;
    void initMainItems();
    void initOptionsItems();
    void initControlsItems();
    void initHighScores();
};

#endif // MENU_H
