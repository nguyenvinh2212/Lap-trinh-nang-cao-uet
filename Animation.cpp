#include "Animation.h"
Animation::Animation(){};
Animation::Animation(const std::string& texturePath, SDL_Renderer* renderer,int numFrames,
                     int frameWidth, int frameHeight, float gap, Uint32 frameDelay, bool repeat)
    : numFrames(numFrames), currentFrame(0), frameWidth(frameWidth),
      frameHeight(frameHeight), gap(gap) ,timeSinceLastFrame(0),
      frameDelay(frameDelay) , repeat(repeat)
{

    texture = loadTexture(texturePath.c_str());
    frameClips = new SDL_Rect[numFrames];
    for (int i = 0; i < numFrames; i++) {
        float xPos = i * (frameWidth + gap);
        float yPos = 0;
        frameClips[i].x = xPos;
        frameClips[i].y = yPos;
        frameClips[i].w = frameWidth;
        frameClips[i].h = frameHeight;
    }
}

Animation::~Animation() {
    if (frameClips) {
        delete[] frameClips;
        frameClips = nullptr;
    }
}
void Animation::update(Uint32 deltaTime) {
    timeSinceLastFrame += deltaTime;
    if (timeSinceLastFrame >= frameDelay) {
        timeSinceLastFrame = 0;
        if (repeat) {
            currentFrame++;
            if (currentFrame >= numFrames) currentFrame = 0;
        }
        else {
            if (currentFrame < numFrames - 1) {
                currentFrame++;
            }
        }
    }
}
void Animation::render(SDL_Renderer* renderer, float x, float y) {
    blitRect(texture, &frameClips[currentFrame], x, y);
}

