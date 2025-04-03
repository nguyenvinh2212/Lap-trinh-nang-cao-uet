
#ifndef Game_object_h
#define Game_object_h
#include "Animation.h"
#include "Common.h"

extern void cleanupStage();
extern Uint32 deltaTime;
class Entity // lớp cơ sở
{
public :
    float x, y, dx, dy, time;
    int w, h, health, reload , side;
     int powerupType;
    SDL_Texture* texture;
    Entity(float x = 0 , float y = 0,float dx =0, float dy = 0,
        int health = 0, SDL_Texture* tex = nullptr)
        : x(x), y(y), dx(dx),dy(dy), health(health), texture(tex) {}
    virtual~Entity() {}
    virtual void update(){x += dx;y += dy;}
};


class Player : public Entity // Lớp player kế thừa Entity
{
public:
    int reload;
    bool hasShield;
    float shieldTimer;
    Animation* shieldAnimation;
    SDL_Texture* playerTexture = loadTexture("Image/player.png");
    Animation* planet = nullptr;
    Animation* playerTail = nullptr ;
    Player();
    ~Player();
    virtual void update();  // Ghi đè hàm update
    void clip();            // giới hạn chuyển động
    void applyShield();     // Hàm để áp dụng hiệu ứng khiên
};


class Explosion : public Entity
{
public :
    int a, r, g, b;             // Giá trị alpha, màu cho explosion
    virtual void update() override {a--;}

};
class Debris : public Entity
{
public :
    SDL_Rect rect;
    virtual void update() override
    {
        x += dx;
        y += dy +5;
    }
};
class Enemy : public Entity
{
public :
    SDL_Texture* enemyTexture = loadTexture("Image/enemy1.png");
    int wave;
    int healthArr[15] = {1, 2, 2, 3, 3, 4, 4, 25, 5, 5, 5, 6, 6, 6 ,50}; // Mảng giá trị máu của kẻ địch
    Enemy(int i = 1);
    virtual void update() override;
};


class Stage //quản lý trạng thái trò chơi
{
public:
    int score , wave = 7, coint ;
    int enemySpawnTimer = 0;
    int StageRestTimer = FPS *3;
    Player* player = nullptr;

    bool victory;
    bool gameOver;
    std::vector<Entity*> bullets;
    std::vector<Explosion*> explosions;
    std::vector<Debris*> debris;
    std::vector<Entity*> PowerUp;
    std::vector<Enemy*> enemies;
    Stage(){}
    ~Stage(){cleanupStage();};
    void reset();               // Hàm để đặt lại màn chơi
    void initPlayer();          // Hàm để khởi tạo người chơi
    void fireBullet();          // Hàm để bắn đạn của người chơi
    void doPlayer();            // Hàm để cập nhật logic của người chơi
    void fireAlienBullet(Enemy* e); // Hàm để bắn đạn của kẻ địch
    void doFighters();          // Hàm để cập nhật logic của kẻ địch
    void doBullets();           // Hàm để cập nhật logic của đạn
    bool checkBulletHit(Entity* e = nullptr, Player* p = nullptr); // Hàm để kiểm tra va chạm đạn
    void spawnEnemies();        // Hàm để spawn kẻ địch
    void doExplosions();        // Hàm để cập nhật logic của vụ nổ
    void doDebris();            // Hàm để cập nhật logic của mảnh vỡ
    void doPowerUp();           // Hàm để cập nhật logic của power-up
    void addExplosions(int x, int y, int num); // Hàm để thêm vụ nổ
    void addDebris(Enemy* e);   // Hàm để thêm mảnh vỡ
    void addPowerUp(int x, int y, int type); // Hàm để thêm power-up
    void logic();               // Hàm để xử lý logic game
    void draw();                // Hàm để vẽ các thành phần game
};


extern Stage stage;
#endif



