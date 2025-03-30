
#include "Graphic.h"
#include "Highscore.h"
std::unordered_map<std::string, SDL_Texture*> textureCache;
extern void cleanupStage();
extern HighScoreManager* HighScore_Manager;
extern TextRenderer* textManager;
int soundVolume = 100;
int musicVolume = 100;

//  Lấy texture từ cache
SDL_Texture* getTexture(const char* name) {
    std::unordered_map<std::string, SDL_Texture*>::iterator it = textureCache.find(name);
    if (it != textureCache.end()) {
        return it->second;
    }
    return nullptr;
}
//  Tải texture từ file (nếu chưa có trong cache thì load)
SDL_Texture* loadTexture(const char* filename) {
    SDL_Texture* texture = getTexture(filename);
    if (texture == nullptr)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
        texture = IMG_LoadTexture(app.renderer, filename);
         textureCache[std::string(filename)] = texture; // Lưu vào cache
    }
    if (texture == nullptr)
        logErrorAndExit("Failed to loadTexture ", filename);
    return texture;
}

// Giải phóng toàn bộ texture trong cache
void clearTextureCache() {
    for (auto& pair : textureCache) {
        SDL_DestroyTexture(pair.second);
    }
    textureCache.clear();
}

void close()
{
    if (HighScore_Manager) {
        delete HighScore_Manager;
        HighScore_Manager = nullptr;
    }
    if (textManager) {
        delete textManager;
        textManager = nullptr;
    }

    if(app.renderer)
    {
        SDL_DestroyRenderer(app.renderer);
        app.renderer = nullptr;
    }
    if(app.window)
    {
        SDL_DestroyWindow(app.window);
        app.window = nullptr;
    }
    clearTextureCache();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void prepareScene()
{
	SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene()
{
	SDL_RenderPresent(app.renderer);
}

void blit(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, float x, float y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;
	SDL_RenderCopy(app.renderer, texture, src, &dest);
}

// Text
TextRenderer::TextRenderer(const std::string& fontPath, int fontsize)
{
    if(TTF_Init() == -1) logErrorAndExit("Failed to init SDL_ttf", TTF_GetError());
    this->fontPath = fontPath;
    font = TTF_OpenFont(fontPath.c_str(), fontsize);
    if(!font) logErrorAndExit("Failed to load font",TTF_GetError());
}
void TextRenderer::drawText(const std::string& text, int x, int y, int r, int g, int b, int align)
{
    SDL_Color color = {Uint8(r), Uint8(g), Uint8(b), 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if(textSurface == nullptr){
        logErrorAndExit("Failed to create text surface", TTF_GetError());
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app.renderer, textSurface);
    if(textTexture == nullptr)
    {
        logErrorAndExit("Failed to create text texture", SDL_GetError());
        return;
    }
    SDL_FreeSurface(textSurface);
    int textW, textH;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
    if (align == TEXT_CENTER)
        x -= textW / 2;
    else if (align == TEXT_RIGHT)
        x -= textW;
    SDL_Rect textRect = {x, y, textW, textH};
    SDL_RenderCopy(app.renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}

void TextRenderer::setFontSize(int fontsize)
{

    if (font) TTF_CloseFont(font); // Giải phóng font cũ
    font = TTF_OpenFont(fontPath.c_str(), fontsize);
    if (!font) logErrorAndExit("Failed to change font size", TTF_GetError());
}


//  Sound
SoundManager::SoundManager() : music(nullptr) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << std::endl;
        return;
    }
    sounds.resize(SND_MAX, nullptr);
    loadSounds();
}

SoundManager::~SoundManager() {
    for (auto sound : sounds) {
        if (sound) Mix_FreeChunk(sound);
    }
    if (music) Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void SoundManager::loadMusic(const std::string& filename) {
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }
    music = Mix_LoadMUS(filename.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    }
}

void SoundManager::playMusic(bool loop) {
    if (music) {
        Mix_PlayMusic(music, loop ? -1 : 0);
    }
}

void SoundManager::playSound(int id, int channel) {
    if (id >= 0 && id < (int)sounds.size() && sounds[id]) {
        Mix_PlayChannel(channel, sounds[id], 0);
    }
}

void SoundManager::loadSounds() {
    std::vector<std::string> soundFiles = {
        "sound/334227__jradcoolness__laser.ogg",
        "sound/196914__dpoggioli__laser-gun.ogg",
        "sound/245372__quaker540__hq-explosion.ogg",
        "sound/10 Guage Shotgun-SoundBible.com-74120584.ogg",
        "sound/mixkit-space-coin-win-notification-271.wav",
    };
    for (size_t i = 0; i < soundFiles.size(); i++) {
        sounds[i] = Mix_LoadWAV(soundFiles[i].c_str());
        if (!sounds[i]) {
            std::cerr << "Failed to load sound " << i << " (" << soundFiles[i] << "): " << Mix_GetError() << std::endl;
        }
    }
}


