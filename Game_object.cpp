
#include "Game_object.h"

// player
Player::Player()
{
    texture = playerTexture;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    x = SCREEN_WIDTH / 2 - w / 2;
    y = 620;
    health = 3;
    reload = 8;
    dx = 0;
    dy = 0;
    side = SIDE_PLAYER; // gán phe
    playerTail = new Animation("animation/Nairan - Frigate - Engine.png", app.renderer, 8, 32, 10, 58,30); // đuôi lửa
    planet = new Animation("animation/Earth-Like planet.png", app.renderer, 60, 62, 64, 34, 100);
    hasShield = false;
    shieldTimer = 0;
    shieldAnimation = nullptr;
}
// destructor
Player::~Player()
{
    if(playerTail)
    {
        delete playerTail;
        playerTail = nullptr;
    }
    if(planet)
    {
       delete planet;
        planet = nullptr ;
    }
    if(shieldAnimation)
    {
        delete shieldAnimation;
        shieldAnimation = nullptr;
    }
}
// updatet vị trí của player
void Player::update()
{
    x += dx;
    y += dy;
    clip();
    if (playerTail)
        playerTail->update(deltaTime);
    if (hasShield) {
        shieldTimer -= (float)deltaTime / 1000;
        if (shieldTimer <= 0) {
            hasShield = false;
            shieldTimer = 0;
        } else {
            if (shieldAnimation)
                shieldAnimation->update(deltaTime);
        }
    }
}
// giới hạn di chuyển
void Player::clip()
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > SCREEN_WIDTH - w) x = SCREEN_WIDTH - w;
    if (y > SCREEN_HEIGHT - h) y = SCREEN_HEIGHT - h;
}
// kích hoạt khiên
void Player::applyShield()
{
    hasShield = true;
    shieldTimer = 10;
    if (!shieldAnimation) {
        shieldAnimation = new Animation("animation/Shield_animation.png", app.renderer, 11, 68, 68, 16.5 , 100, true);
    }
}

// Ennemy
Enemy::Enemy(int wave)
{
    side = SIDE_ALIEN;
    this->wave = wave;
    health = healthArr[wave]; // lấy máu theo mỗi wave
    if(wave == 7 || wave == 14) reload = FPS;
    else reload = FPS * (4 + rand() % 3);
    // load texture và cách đi cho từng wave
    switch (wave % 7 ) {
        case 1: // Đi ziczac ngang
            dx = (SDL_GetTicks() / 300 % 2 == 0) ? 1.5 : -1.5;
            dy = 0.5;
            texture = loadTexture("Image/enemy1.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
        case 2: // Lượn sóng
            dx = sin(SDL_GetTicks() / 200.0) * 3;
            dy = 1.0;
            texture = loadTexture("Image/enemy2.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
        case 3: // Đi chéo trái
            dx = -1.2;
            dy = 0.5;
            texture = loadTexture("Image/enemy1.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
        case 4: // Đi chéo phải
            dx = 1.2;
            dy = 2.5;
            texture = loadTexture("Image/enemy2.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
        case 5: // Hình sin mạnh
            dx = sin(SDL_GetTicks() / 100.0) * 3.5;
            dy = 1.5;
            texture = loadTexture("Image/enemy3.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
        case 6: // Đi chậm dần
            dx = rand() % 5 - rand() % 5;
            dy = std::max(0.5, 1.8 - SDL_GetTicks() / 2000.0);
            texture = loadTexture("Image/enemy4.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
        case 0:
            y = 100;
            dx = 2;
            dy = 0.1;
            texture = loadTexture("Image/boss.png");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            break;
    }
}
// update vị trí và chuyển động khi va chạm
void Enemy::update()
{
    if (x <= 0) // chạm bên trái
    {
        dx=-dx;
        if(dx == 0 || dx < 0) x = 0 + rand() % 40;
    }
    if (x >= SCREEN_WIDTH - w ) // chạm phải
    {
        dx = -dx;
        if(dx == 0 || dx > 0) x = SCREEN_WIDTH - w - rand() % 40;
    }
    if(wave %7 == 4 || wave % 7 == 5) //wave boss
    {
        if(y >= SCREEN_HEIGHT - h/2)
        {
            dx = -dx - rand() % 2;
            dy = -dy ;
        }
        if(y < 0 + w)
        {
            dx = 2 + rand() % 2;
            dy = 2.5;
        }
    }
    if(y > SCREEN_HEIGHT + h) health = 0;
    x += dx;
    y += dy;

}
