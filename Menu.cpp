#include "Menu.h"
#include "Highscore.h"
#include "Graphic.h"

extern void doBackground();
extern void drawBackground();
extern App app;
extern bool gameState;
extern Menu menu ;
extern HighScoreManager* HighScore_Manager;
extern int soundVolume;
extern int musicVolume;
extern TextRenderer* textManager;
void menuOptions() {
    menu.switchToOptions();
}
// chuyển đổi giữa các menu
void menuMain() {
    menu.switchToMain();
}
void menuControls()
{
    menu.switchToControls();
}
void menuHighscore()
{
    menu.switchToHighScores();
}
// vào game
void startGame()
{
    gameState = true;
}
// thoát game
void exitGame() {
    app.running = false;
}

void Menu::switchToOptions() {
    inOptionsMenu = true;
    inHighScoresMenu = false;
    inControlsMenu = false;
    initOptionsItems();
    activeIndex = 0;
}

void Menu::switchToMain() {
    inHighScoresMenu = false;
    inOptionsMenu = false;
    inControlsMenu = false;
    initMainItems();
    activeIndex = 0;
}
void Menu::switchToControls()
{
    inOptionsMenu = false;
    inHighScoresMenu = false;
    inControlsMenu = true;
    initControlsItems();
    activeIndex = 0;
}
void Menu::switchToHighScores()
{
    inOptionsMenu = false;
    inControlsMenu = false;
    inHighScoresMenu = true;
    initHighScores();
    activeIndex = 0;
}
// constructor
Menu::Menu() : activeIndex(0), active(true), title(nullptr), inOptionsMenu(false), inControlsMenu(false), inHighScoresMenu(false){}

// load title
void Menu::init() {
    active = true;
    activeIndex = 0;
    title = loadTexture("Image/Title.png");
    initMainItems();
}

// Khởi tạo các item của Menu chính
void Menu::initMainItems() {
    items.clear();
    activeIndex = 0;
    MenuItem item;

    // Start
    item.label = "Start";
    item.x = 250;
    item.y = 300;
    item.normal   = loadTexture("Image/menu/start.png");
    item.selected = loadTexture("Image/menu/start_selected.png");
    item.action   = startGame;
    items.push_back(item);

    // Options
    item.label = "Options";
    item.x = 250;
    item.y = 350;
    item.normal   = loadTexture("Image/menu/option.png");
    item.selected = loadTexture("Image/menu/option_selected.png");
    item.action   = menuOptions;
    items.push_back(item);

    // Controls
    {
        MenuItem item;
        item.label = "Controls";
        item.x = 250;
        item.y = 400;
        item.normal   = loadTexture("Image/menu/controls.png");
        item.selected = loadTexture("Image/menu/controls_selected.png");
        item.action   = menuControls;
        items.push_back(item);
    }

    // Highscore
    {
        MenuItem item;
        item.label = "highscore";
        item.x = 250;
        item.y = 450;
        item.normal   = loadTexture("Image/menu/highscore.png");
        item.selected = loadTexture("Image/menu/highscore_selected.png");
        item.action   = menuHighscore;
        items.push_back(item);
    }

    // Exit
    item.label = "Exit";
    item.x = 250;
    item.y = 500;
    item.normal   = loadTexture("Image/menu/exit.png");
    item.selected = loadTexture("Image/menu/exit_selected.png");
    item.action   = exitGame;
    items.push_back(item);
}

// Menu Options
void Menu::initOptionsItems() {
    items.clear();
    activeIndex = 0;

    // Sound
    {
        MenuItem item;
        item.label = "Sound";
        item.x = 200; // Tọa độ "danh nghĩa" cho nút
        item.y = 300;
        item.normal   = loadTexture("Image/menu/sound.png");
        item.selected = loadTexture("Image/menu/sound_selected.png");
        item.action = nullptr;
        items.push_back(item);
    }

    // Music
    {
        MenuItem item;
        item.label = "Music";
        item.x = 200;
        item.y = 360;
        item.normal   = loadTexture("Image/menu/music.png");
        item.selected = loadTexture("Image/menu/music_selected.png");
        item.action = nullptr;
        items.push_back(item);
    }
    // Back
    {
        MenuItem item;
        item.label = "Back";
        item.x = 200;
        item.y = 420;
        item.normal   = loadTexture("Image/menu/back.png");
        item.selected = loadTexture("Image/menu/back_selected.png");
        item.action   = menuMain;
        items.push_back(item);
    }
}
//  MenuControls
void Menu::initControlsItems() {
    items.clear();
    activeIndex = 0;
    MenuItem item;
    // Left
    item.label = "Left";
    item.x = 150;
    item.y = 300;
    item.normal   = loadTexture("Image/menu/left.png");
    item.selected = loadTexture("Image/menu/left_selected.png");
    item.action   = nullptr; // Nếu bấm Enter không làm gì
    items.push_back(item);

    // Right
    item.label = "Right";
    item.x = 150;
    item.y = 350;
    item.normal   = loadTexture("Image/menu/right.png");
    item.selected = loadTexture("Image/menu/right_selected.png");
    item.action   = nullptr;
    items.push_back(item);

    // Up
    item.label = "Up";
    item.x = 150;
    item.y = 400;
    item.normal   = loadTexture("Image/menu/up.png");
    item.selected = loadTexture("Image/menu/up_selected.png");
    item.action   = nullptr;
    items.push_back(item);

    // Down
    item.label = "Down";
    item.x = 150;
    item.y = 450;
    item.normal   = loadTexture("Image/menu/down.png");
    item.selected = loadTexture("Image/menu/down_selected.png");
    item.action   = nullptr;
    items.push_back(item);

    // Fire
    item.label = "Fire";
    item.x = 150;
    item.y = 500;
    item.normal   = loadTexture("Image/menu/fire.png");
    item.selected = loadTexture("Image/menu/fire_selected.png");
    item.action   = nullptr;
    items.push_back(item);

    // Back
    item.label = "Back";
    item.x = 150;
    item.y = 550;
    item.normal   = loadTexture("Image/menu/back_controls.png");
    item.selected = loadTexture("Image/menu/back_controls_selected.png");
    item.action   = menuMain;
    items.push_back(item);
}
// Highscore
void Menu::initHighScores()
{
    items.clear();
    activeIndex = 0;
    MenuItem item;
    item.label = "Back";
    item.x = 250;
    item.y = 480;
    item.normal   = loadTexture("Image/menu/back.png");
    item.selected = loadTexture("Image/menu/back_selected.png");
    item.action   = menuMain;
    items.push_back(item);
}

// Update
void Menu::update() {
    // Chọn items
    if (app.keyboard[SDL_SCANCODE_UP]) {
        activeIndex--;
        if (activeIndex < 0) activeIndex = items.size() - 1;
        app.keyboard[SDL_SCANCODE_UP] = 0;
    }
    if (app.keyboard[SDL_SCANCODE_DOWN]) {
        activeIndex++;
        if (activeIndex >= (int)items.size()) activeIndex = 0;
        app.keyboard[SDL_SCANCODE_DOWN] = 0;
    }

    // Nếu Sound hoặc Music xử lý tăng giảm âm lượng
    std::string currentLabel = items[activeIndex].label;
    if (currentLabel == "Sound")
    {
        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            soundVolume -= 5;
            if (soundVolume < 0) soundVolume = 0;
            app.keyboard[SDL_SCANCODE_LEFT] = 0;
        }
        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            soundVolume += 5;
            if (soundVolume > 100) soundVolume = 100;
            app.keyboard[SDL_SCANCODE_RIGHT] = 0;
        }
    }

    else if (currentLabel == "Music")
    {
        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            musicVolume -= 5;
            if (musicVolume < 0) musicVolume = 0;
            app.keyboard[SDL_SCANCODE_LEFT] = 0;
        }
        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            musicVolume += 5;
            if (musicVolume > 100) musicVolume = 100;
            app.keyboard[SDL_SCANCODE_RIGHT] = 0;
        }
    }

    // Xử lý phím ENTER
    if (app.keyboard[SDL_SCANCODE_RETURN]) {
        if (!items.empty() && items[activeIndex].action != nullptr) {
            items[activeIndex].action();
        }
        app.keyboard[SDL_SCANCODE_RETURN] = 0;
    }
}

// Draw
void Menu::draw()
{
    doBackground();
    drawBackground();
    if (title && !inHighScoresMenu)
    {
        SDL_Rect titleRect = { 65, 120, 600, 150 };
        SDL_RenderCopy(app.renderer, title, NULL, &titleRect);
    }

     if (inHighScoresMenu)
     {
        textManager->setFontSize(50);
        textManager->drawText("High Scores", SCREEN_WIDTH/2, 100, 255, 255, 255, TEXT_CENTER);
        textManager->setFontSize(30);
        const std::vector<int>& scores = HighScore_Manager->getHighScores();
        for (size_t i = 0; i < scores.size(); i++)
        {
            std::string line = std::to_string(i+1) + ". " + std::to_string(scores[i]);
            textManager->drawText(line, 280, 180 + i * 40, 255, 255, 0, TEXT_LEFT);
        }
     }

    for (int  i = 0; i < (int)items.size(); i++)
    {
        SDL_Texture* tex = (i == activeIndex) ? items[i].selected : items[i].normal;
        SDL_Rect dest;
        if (inControlsMenu)
            dest = {items[i].x, items[i].y, 400, 40};
        else
            dest = { items[i].x, items[i].y, 150, 40};
        SDL_RenderCopy(app.renderer, tex, NULL, &dest);

        //  nếu Sound hoặc Music vẽ rect hiển thị độ lớn âm thanh .
        if (items[i].label == "Sound")
        {
            int barX = items[i].x + 160;
            int barY = items[i].y + 10;
            int barW = 200;
            int barH = 20;
            int fillWidth = (barW * soundVolume) / 100;

            SDL_Rect fillRect = { barX, barY, fillWidth, barH };
            SDL_Rect frameRect = { barX, barY, barW, barH };

            if (i == activeIndex)
                SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255); // xanh
            else
                SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255); // trắng

            SDL_RenderFillRect(app.renderer, &fillRect);
            SDL_RenderDrawRect(app.renderer, &frameRect);
        }
        else if (items[i].label == "Music")
        {
            int barX = items[i].x + 160;
            int barY = items[i].y + 10;
            int barW = 200;
            int barH = 20;
            int fillWidth = (barW * musicVolume) / 100;
            // set thanh rect tăng giảm
            SDL_Rect fillRect = { barX, barY, fillWidth, barH };
            SDL_Rect frameRect = { barX, barY, barW, barH };

            if (i == activeIndex)
                SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255); // xanh
            else
                SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255); // trắng

            SDL_RenderFillRect(app.renderer, &fillRect);
            SDL_RenderDrawRect(app.renderer, &frameRect);
        }
    }
}
