

#include "Common.h"
#include <algorithm>

// xử lý sự kiện vào từ bàn phím
void doInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) // lấy hết sự kiện hiện có ở hàng đợi
    {
        switch (event.type)
        {
            case SDL_QUIT: // khi đóng của sổ
                app.running = false;
                break;
            case SDL_KEYDOWN:// khi phím nhấn
                if (event.key.repeat == 0 && event.key.keysym.scancode < MAX_KEYBOARD_KEYS)
                    app.keyboard[event.key.keysym.scancode] = 1;
                break;
            case SDL_KEYUP: // khi nhả
                if (event.key.repeat == 0 && event.key.keysym.scancode < MAX_KEYBOARD_KEYS)
                    app.keyboard[event.key.keysym.scancode] = 0;
                break;
            default:
                break;
        }
    }
}
// check va chạm
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (std::max(x1, x2) <= std::min(x1 + w1, x2 + w2)) && (std::max(y1, y2) <= std::min(y1 + h1, y2 + h2));
}
