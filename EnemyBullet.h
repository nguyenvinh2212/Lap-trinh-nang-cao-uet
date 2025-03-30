#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H
#include <vector>
#include "Game_object.h"

class EnemyBullet {
public:
    EnemyBullet();
    std::vector<Entity*> fireBullet(SDL_Texture* bulletTexture, int wave, int enemyX, int enemyY, int enemyW, int enemyH);

private:
    int getBulletLevel(int wave);
};

#endif
