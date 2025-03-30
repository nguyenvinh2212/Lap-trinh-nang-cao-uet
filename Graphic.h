
#ifndef Graphic_h
#define Graphic_h
#include "Common.h"
#include <unordered_map>
// text
class TextRenderer
{
private:
    TTF_Font* font = nullptr;
    std::string fontPath;
public:
    TextRenderer(const std::string& fontPath, int fontsize);
    ~TextRenderer()
    {
        if(font) TTF_CloseFont(font);
        font = nullptr;
    }
    void drawText(const std::string& text, int x, int y, int r, int g, int b, int align);
    void setFontSize(int fontsize);
};
// sound
class SoundManager
{
public:
    SoundManager();
    ~SoundManager();
    void loadMusic(const std::string& filename);
    void playMusic(bool loop);
    void playSound(int id, int channel = -1);
private:
    std::vector<Mix_Chunk*> sounds;
    Mix_Music* music;
    void loadSounds();
};
extern void logErrorAndExit(const char* msg, const char* error);

#endif // Graphic_h
