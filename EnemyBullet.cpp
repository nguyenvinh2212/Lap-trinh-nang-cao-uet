#include "EnemyBullet.h"
#include <SDL2/SDL.h>
#include <cmath>
#include "def.h"

EnemyBullet::EnemyBullet() {}

int EnemyBullet::getBulletLevel(int wave) {
    if (wave == 7)
        return 5;
    else if(wave == 14)
        return 7;
    return 1;
}

std::vector<Entity*> EnemyBullet::fireBullet(SDL_Texture* bulletTexture, int wave, int enemyX, int enemyY, int enemyW, int enemyH) {
    std::vector<Entity*> newBullets;
    int bulletW, bulletH;
    SDL_QueryTexture(bulletTexture, nullptr, nullptr, &bulletW, &bulletH);

    int bulletLevel = getBulletLevel(wave);

    int numBullets = bulletLevel;
    float angleStep = 15;
    float startAngle = -((numBullets - 1) * angleStep) / 2;

    for (int i = 0; i < numBullets; i++) {
        float angle = startAngle + i * angleStep;
        float rad = angle * 3.14 / 180;


        Entity* bullet = new Entity();
        bullet->texture = bulletTexture;
        bullet->w = bulletW;
        bullet->h = bulletH;

        bullet->x = enemyX + enemyW / 2 - bulletW / 2;
        bullet->y = enemyY + enemyH;

        float speed = ENEMY_BULLET_SPEED;
        bullet->dx = speed * sin(rad);
        bullet->dy = speed * cos(rad);
        bullet->side = SIDE_ALIEN;
        bullet->health = 1;

        newBullets.push_back(bullet);
    }

    return newBullets;
}
