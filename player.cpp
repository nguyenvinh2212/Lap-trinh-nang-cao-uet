
#include "player.h"
/*
Player::Player() : tailAnim(nullptr) {}
// Sửa lại constructor để khởi tạo tailAnim
Player::Player(SDL_Texture* texture_) : tailAnim(nullptr) {
    texture = texture_;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    x = SCREEN_WIDTH / 2 - w / 2;
    y = 620;
    health = 3;
    reload = 8;
    dx = 0;
    dy = 0;
    side = SIDE_PLAYER;
        // Khởi tạo tal animation:
        // Giả sử sprite sheet tail có đường dẫn "Image/tail.png"
        // Với 8 frame, mỗi frame 32x8, gap = 56
    tailAnim = new Animation("Image/tail.png", app.renderer, 8, 32, 8, 56);
        // Nếu bạn muốn lặp lại animation, thì mặc định tailAnim đã lặp
}
virtual Player::~Player() {
    if (tailAnim) {
        delete tailAnim;
        tailAnim = nullptr;
    }
}
virtual void  Player::update() {
    x += dx;
    y += dy;
    clip();
    // Cập nhật animation đuôi nếu có
    if (tailAnim)
        tailAnim->update();
}
void Player::clip() {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > SCREEN_WIDTH - w) x = SCREEN_WIDTH - w;
    if (y > SCREEN_HEIGHT - h) y = SCREEN_HEIGHT - h;
}
*/
