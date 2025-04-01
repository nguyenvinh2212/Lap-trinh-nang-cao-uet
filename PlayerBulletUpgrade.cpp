#include "PlayerBulletUpgrade.h"
#include <SDL2/SDL.h>
#include <cmath>

PlayerBulletUpgrade::PlayerBulletUpgrade()
    : bulletLevel(1)
{
}
void PlayerBulletUpgrade::increaseBulletLevel()
{
    if (bulletLevel < 7) bulletLevel++;
}
std::vector<Entity*> PlayerBulletUpgrade::fireBullet(SDL_Texture* bulletTexture, Player* player)
{
    std::vector<Entity*> newBullets;
    if(!player)
        return newBullets;
    // Lấy kích thước viên đạn
    int bulletW, bulletH;
    SDL_QueryTexture(bulletTexture, NULL, NULL, &bulletW, &bulletH);
    int numBullets = 1;
    if (bulletLevel == 2) numBullets = 2;
    else if (bulletLevel == 3) numBullets = 3;
    else if (bulletLevel == 4) numBullets = 4;
    else if (bulletLevel == 5) numBullets = 5;
    else if (bulletLevel == 6) numBullets = 6;
    else if  (bulletLevel == 7) numBullets = 7;
    float angleStep = 5; // góc giữa các bullet
    float startAngle = -(numBullets - 1) * angleStep / 2; // vị trí góc bullet đầu tiên
    for (int i = 0; i < numBullets; i++)
    {
        // Góc của viên đạn thứ i
        float angle = startAngle + i * angleStep;
        float rad = angle * 3.14 / 180;

        // Tạo Entity đạn
        Entity* bullet = new Entity();
        bullet->texture = bulletTexture;
        bullet->w = bulletW;
        bullet->h = bulletH;

        // Đặt vị trí ban đầu của đạn (chính giữa Player)
        bullet->x = player->x + player->w / 2 - bulletW / 2;
        bullet->y = player->y;

        float speed = PLAYER_BULLET_SPEED; // dùng hằng số của bạn
        bullet->dx = speed * sin(rad);
        bullet->dy = -speed * cos(rad);

        bullet->side = SIDE_PLAYER;
        bullet->health = 1;
        newBullets.push_back(bullet);
    }

    return newBullets;
}
