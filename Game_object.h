
#ifndef Game_object_h
#define Game_object_h
#include "Animation.h"
#include "Common.h"

extern void cleanupStage();
extern Uint32 deltaTime;
class Entity
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


class Player : public Entity
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
    virtual void update();
    void clip();
    void applyShield();
};


class Explosion : public Entity
{
public :
    int a, r, g, b;
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
    int healthArr[15] = {1, 2, 2, 3, 3, 4, 4, 20, 5, 5, 5, 6, 6, 6 ,40};
    Enemy(int i = 1);
    virtual void update() override;
};


class Stage
{
public:
    int score , wave = 7, coint  ;
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
    void reset();
    void initPlayer();
    void fireBullet();
    void doPlayer();
    void fireAlienBullet(Enemy *e);
    void doFighters();
    void doBullets();
    bool checkBulletHit(Entity* e = nullptr, Player* p = nullptr);
    void spawnEnemies();
    void doExplosions();
    void doDebris();
    void doPowerUp();
    void addExplosions(int x, int y, int num);
    void addDebris(Enemy* e);
    void addPowerUp(int x, int y, int type);
    void logic();
    void draw();
};


extern Stage stage;
#endif



