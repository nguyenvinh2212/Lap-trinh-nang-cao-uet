#ifndef PLAYER_BULLET_UPGRADE_H
#define PLAYER_BULLET_UPGRADE_H

#include "Game_object.h"
#include <vector>

class PlayerBulletUpgrade {
public:
    PlayerBulletUpgrade();
    std::vector<Entity*> fireBullet(SDL_Texture* bulletTexture, Player* player);
    void increaseBulletLevel();
    void resetLevel(){bulletLevel = 1;}
private:
    int bulletLevel;
};

#endif // PLAYER_BULLET_UPGRADE_H
