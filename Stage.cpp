#include "Common.h"
#include "Stage.h"
#include "EnemyBullet.h"
#include "Highscore.h"


static int highscore = 0;
static SDL_Texture* bulletTexture = nullptr;
static SDL_Texture* enemyTexture = nullptr;
static SDL_Texture* EnemyBulletTexture =nullptr ;
static SDL_Texture* explosionTexture = nullptr;
static SDL_Texture* pointsTexture = nullptr;
static SDL_Texture* bulletPowerTexture = nullptr;
static SDL_Texture* lifeTexture = nullptr;
static SDL_Texture* gameOverTexture = nullptr;
static SDL_Texture* victoryTexture = nullptr;
static bool menuChoice = true;
static int waveDelayTimer = FPS * 3;
static int enemiesRemainingInWave = 0;
static bool waveInProgress = false;

PlayerBulletUpgrade* bulletUpgrade = new PlayerBulletUpgrade();
Animation* BulletEnemyAnimation = nullptr;
Animation* CoinsAnimation = nullptr;
EnemyBullet enemyBullet;
extern HighScoreManager* HighScore_Manager;
void initStage()
{
    bulletTexture = loadTexture("Image/playerBullet.png");
    enemyTexture = loadTexture("Image/enemy1.png");
	EnemyBulletTexture = loadTexture("Image/alienBullet.png");
	explosionTexture = loadTexture("Image/explosion.png");
	pointsTexture = loadTexture("Image/points.png");
    bulletPowerTexture = loadTexture("Image/Powerup_bullet.png");
    lifeTexture = loadTexture("Image/heart.png");
    gameOverTexture = loadTexture("Image/GameOver.png");
    victoryTexture = loadTexture("Image/Victory.png");
	stage.reset();
	stage.score = 0;
	stage.coint = 0;
	stage.initPlayer();
	stage.enemySpawnTimer = 0;
    stage.StageRestTimer= FPS * 3;
    BulletEnemyAnimation = new Animation("animation/bulletEnemy.png", app.renderer, 3, 16 ,16 ,3, 50, true);
    CoinsAnimation = new Animation("animation/CoinsAnimation.png", app.renderer, 8, 31, 32, 1, 50, true);
}
void Stage::reset()
{
    for(Enemy* i : enemies) delete i;
    enemies.clear();
    for(Entity* i : bullets) delete i;
    bullets.clear();
    for(Entity* i : explosions) delete i;
    explosions.clear();
    for(Debris* i : debris) delete i;
    debris.clear();
    for(Entity* i : PowerUp) delete i;
    PowerUp.clear();
    if (player) {
        delete player;
        player = nullptr;
    }
    victory = false;
    gameOver = false;
    score = 0;
    wave = 1;
    bulletUpgrade->resetLevel();
    waveDelayTimer = FPS * 3;
    enemiesRemainingInWave = 0;
    waveInProgress = false;
}
void Stage::initPlayer()
{
    player = new Player();
}
void Stage::fireBullet()
{
    std::vector<Entity*> bulletsToAdd = bulletUpgrade->fireBullet(bulletTexture, player);
    for (Entity* b : bulletsToAdd) {
        bullets.push_back(b);
    }
    player->reload = 8;
}
void Stage::doPlayer()
{
    if (!player) return;
    player->dx = player->dy = 0;
    if (player->reload > 0) player->reload --;
    if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0)
		{
			sound.playSound(SND_PLAYER_FIRE, CH_PLAYER);
			fireBullet();
		}
}

void Stage::fireAlienBullet(Enemy* e)
{
    std::vector<Entity*> enemyBullets = enemyBullet.fireBullet(EnemyBulletTexture, e->wave, e->x, e->y, e->w, e->h);
    if(wave == 7 || wave == 14) e->reload = rand() % FPS + 20;
    else e->reload = FPS*(3 + rand() % 2);
    for (Entity* b : enemyBullets) bullets.push_back(b);
}
void Stage::doFighters()
{
    if (player) {
        player->update();
        player->planet->update(deltaTime);
        planet_y += 0.2;
        for (Enemy* e : enemies)
        {
            if (collision(player->x, player->y, player->w, player->h, e->x, e->y, e->w, e->h))
            {
                addExplosions(player->x, player->y, 10);
                if(!player->hasShield) player->health--;
                e->health = 0;
            }
        }
    }
    if(player && player->health <= 0)
    {
        delete player;
        player = nullptr;
    }

    for(int i =0 ;i < (int)enemies.size();i++)
    {
        Enemy* e = enemies[i];
        e->update();
        e->reload --;
        if(player && (e->reload) <= 0)
        {
            fireAlienBullet(e);
            sound.playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
        }
        if(e->health <= 0)
        {
            delete e;
            std::swap(enemies[i], enemies.back());
            enemies.pop_back();
            i--;
        }
    }
}


void Stage::doBullets()
{
    for(int i = 0; i < (int)bullets.size(); i++)
    {
        Entity *b = bullets[i];
        b->update();
        if(b->side == SIDE_ALIEN) BulletEnemyAnimation->update(deltaTime);
        if (checkBulletHit(b) ||checkBulletHit(b, player) || b->x < -b->w || b->y < -b->h ||
            b->x > SCREEN_WIDTH  || b->y > SCREEN_HEIGHT)
        {
            delete b;
            std::swap(bullets[i],bullets.back());
            bullets.pop_back();
            i--;
        }
    }
}
bool Stage::checkBulletHit(Entity* b, Player* p)
{
    if(p)
    {
         if(p->side != b->side && collision(b->x, b->y, b->w, b->h, p->x, p->y, p->w, p->h))
         {
            b->health = 0;
            if(!p->hasShield) p->health --;
            addExplosions(p->x, p->y, 10);
            sound.playSound(SND_PLAYER_DIE, CH_PLAYER);
            return true;
         }
    }
    else
    {
        for(int i = 0; i< (int)enemies.size(); i++)
        {
            Enemy* e = enemies[i];
            if(e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
            {
                e->health -= b->health;
                b->health = 0;
                addExplosions(e->x, e->y, 10);
                if(e->health == 0) addDebris(e);
                if(e->health == 0 )
                {
                    if(e->wave == 7 || e->wave == 14) score +=100;
                    else score += 10;
                    int r = rand() % 100;
                    if(r < 70) ;
                    else if (r < 85)
                        addPowerUp(e->x + e->w / 2, e->y + e->h / 2, POWERUP_COIN);
                    else if (r < (90 + wave / 2))
                        addPowerUp(e->x + e->w / 2, e->y + e->h / 2, POWERUP_SHIELD);
                    else if (r<96) addPowerUp(e->x + e->w / 2, e->y + e->h / 2, POWERUP_BULLET);
                    else addPowerUp(e->x + e->w / 2, e->y + e->h / 2, POWERUP_LIFE);
                }
                sound.playSound(SND_ALIEN_DIE, CH_ANY);
                return true;
            }
        }
    }
    return false;
}
void Stage::spawnEnemies()
{
    if (!waveInProgress) {
        if (--waveDelayTimer <= 0) {
            waveInProgress = true;
            if(wave == 7 || wave == 14)  enemiesRemainingInWave = 1;
            else enemiesRemainingInWave = wave + 2 + rand()%5;
            enemySpawnTimer = 0;
        }
    }
    if (wave >= 15 ) victory = true;
    if (waveInProgress) {
        if (--enemySpawnTimer <= 0 && enemiesRemainingInWave > 0) {
            int x_start = rand() % (SCREEN_WIDTH - 100) + 60;
            int temp = 1 + rand() % enemiesRemainingInWave;
            for(int i = 0; i < temp; i++)
            {
                int x = x_start + i * 70;
                int y = -50;
                if (x > SCREEN_WIDTH - 60) x -= SCREEN_WIDTH;
                Enemy* e = new Enemy(wave);
                if (x < 20) x += rand()%50 + 20;
                e->x = x;
                e->y = y;
                enemies.push_back(e);
                enemiesRemainingInWave--;
            }
             enemySpawnTimer = 30 + (rand() % FPS);
        }
        if (enemiesRemainingInWave <= 0) {

            if (enemies.size() <= 0) {
                waveInProgress = false;
                waveDelayTimer = FPS * 2;
                wave ++;
            }
        }
    }
}

void Stage::doExplosions()
{
    for (int i = 0; i < (int)explosions.size(); i++)
    {
        Explosion* e = explosions[i];
        e->update();
        if(e->a <= 0)
        {
            delete e;
            std::swap(explosions[i], explosions.back());
            explosions.pop_back();
            i--;
        }
    }
}
void Stage::doDebris()
{
    for(int i = 0; i <(int)debris.size(); i++)
    {
        Debris* d = debris[i];
        d->update();
        if(d->health <= 0)
        {
            delete d;
            std::swap(debris[i], debris.back());
            debris.pop_back();
            i--;
        }
    }
}
void Stage::doPowerUp()
{
    for(int i = 0; i < (int)PowerUp.size(); i++)
    {
        Entity* e = PowerUp[i];
        if(e->x < 0)
        {
            e->x = 0; e->dx = -e->dx;
        }
        if(e->x + e->w >SCREEN_WIDTH )
        {
            e->x = SCREEN_WIDTH - e->w;
            e->dx = -e->dx;
        }
        if(e->y < 0)
        {
            e->y = 0;
            e->dy = -e->dy ;
        }
        if(e->y + e->h >SCREEN_HEIGHT)
        {
            e->y = SCREEN_HEIGHT - e->h;
            e->dy = -e->dy /2 ;
        }
        e->update();
        if(e->texture == pointsTexture) CoinsAnimation->update(deltaTime);
        e->reload --;
        if (player && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))
        {
            e->health = 0;
             switch (e->powerupType)
            {
                case POWERUP_COIN:
                    stage.coint+= 100;
                    sound.playSound(SND_POINTS, CH_POINTS);
                    break;
                case POWERUP_BULLET:
                    if (bulletUpgrade) {
                        bulletUpgrade->increaseBulletLevel();
                    }
                    sound.playSound(SND_POINTS, CH_POINTS);
                    break;
                case POWERUP_SHIELD:
                    if (player) {
                        player->applyShield();
                    }
                    sound.playSound(SND_POINTS, CH_POINTS);
                    break;
                case POWERUP_LIFE:
                    if (player->health < 4) {
                        player->health++;
                        sound.playSound(SND_POINTS, CH_POINTS);
                    }
                    break;
            }
        }
        if(e->health <= 0 || e->reload <=0 )
        {
            delete e;
            std::swap(PowerUp[i], PowerUp.back());
            PowerUp.pop_back();
            i--;
        }
    }
}
void Stage::addExplosions(int x, int y, int num)
{
    for(int i = 0; i < num; i++)
    {
        Explosion* e = new Explosion();
        e->x = x + rand() % 32 - rand() % 32 ;
        e->y = y +rand() % 32 - rand()%32;
        e->dx = (rand()%10 - rand()%10) / 10.0;
        switch (rand()% 4)
        {
            case 0: e-> r = 255;  break;
            case 1: e->r = 255; e->g = 128; break;
            case 2: e->r = 128; e->g =128; e->b = 128; break;
            default: e->r =255; e->g =255; e->b =255; break;
        }
        e->a = rand() % (FPS);
        explosions.push_back(e);
    }
}
void Stage::addDebris(Enemy* e)
{
    int w = e->w / 4;
    int h = e->h / 4;
    if(w == 0 || h ==0 ) return ;
    for (int i = 0; i<= h; i +=h)
    {
        for(int j =0; j <= w; j+=w)
        {
            Debris* d = new Debris();
            d->x = e->x + w ;
            d->y = e->y + h;
            d->dx = rand()%10 - rand() % 10;
            d->dy = 5-rand()%3;
            d->health = FPS * 2 ;
            d->texture = e->texture;
            d->rect.x = i;
			d->rect.y = j;
			d->rect.w = w;
			d->rect.h = h;
            debris.push_back(d);
        }
    }
}
void Stage::addPowerUp(int x, int y, int type)
{
    Entity* e = new Entity(x, y, rand()%7-rand()%7, 3+rand()%3, 1, pointsTexture);
    e->powerupType = type;
    switch (type)
    {
        case POWERUP_COIN:
            e->texture = pointsTexture;
            break;
        case POWERUP_BULLET:
            e->texture = bulletPowerTexture;
            break;
        case POWERUP_SHIELD:
            e->texture = loadTexture("Image/Powerup_Shield.png");
            break;
        case POWERUP_LIFE:
            e->texture = loadTexture("Image/Powerup_health.png");
            break;
    }

    SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);
    e->x -= e->w /2;
    e->y -= e->h /2;
    e->reload = FPS * 6;
    PowerUp.push_back(e);
}


void Stage::logic()
{

    doBackground();
    if (player == nullptr) gameOver = true;
    if(victory || gameOver)
    {
        HighScore_Manager->addScore(score);
        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            menuChoice = 1; // YES
            app.keyboard[SDL_SCANCODE_LEFT] = 0;
        }
        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            menuChoice =  0; // NO
            app.keyboard[SDL_SCANCODE_RIGHT] = 0;
        }
         if (app.keyboard[SDL_SCANCODE_RETURN])
         {
             if (menuChoice == 0) gameState = false;
            highscore = std::max(highscore, score);
            reset();
            initPlayer();
            StageRestTimer = FPS * 3;
            app.keyboard[SDL_SCANCODE_RETURN] = 0;
         }
         return;
    }
    doPlayer();
    doFighters();
    doBullets();
    doExplosions();
    doDebris();
    doPowerUp();
    spawnEnemies();
}

void Stage::draw()
{
    drawBackground();
    if (victory)
    {
        SDL_Rect rect;
        SDL_QueryTexture(victoryTexture, NULL, NULL, &rect.w, &rect.h);
        rect.x = (SCREEN_WIDTH - rect.w) / 2;
        rect.y = (SCREEN_HEIGHT - rect.h) / 2 - 50;
        SDL_RenderCopy(app.renderer, victoryTexture, NULL, &rect);

        SDL_Color yesColor = {255, 255, 255, 255};
        SDL_Color noColor  = {255, 255, 255, 255};
        if (menuChoice == 1) yesColor = {255, 0, 0, 255};
        else    noColor  = {255, 0, 0, 255};
        textManager->drawText("PLAY AGAIN?", SCREEN_WIDTH /2 , rect.y + rect.h -10, 255, 255, 255, TEXT_CENTER);
        textManager->setFontSize(35);
        int yOption = rect.y + rect.h + 30;
        textManager->drawText("YES", SCREEN_WIDTH/2 - 100, yOption, yesColor.r, yesColor.g, yesColor.b, TEXT_CENTER);
        textManager->drawText("NO",  SCREEN_WIDTH/2 + 100, yOption, noColor.r, noColor.g, noColor.b, TEXT_CENTER);
        return;
    }
    if (gameOver)
    {
        SDL_Rect rect;
        rect.w = 400;
        rect.h = 250;
        rect.x = (SCREEN_WIDTH - rect.w) / 2;
        rect.y = (SCREEN_HEIGHT - rect.h) / 2 - 50;
        SDL_RenderCopy(app.renderer, gameOverTexture, NULL, &rect);

        SDL_Color yesColor = {255, 255, 255, 255};
        SDL_Color noColor  = {255, 255, 255, 255};

        if (menuChoice == 1) yesColor = {255, 0, 0, 255};

        else  noColor  = {255, 0, 0, 255};

        int yOption = rect.y + rect.h + 30;
        textManager->drawText("YES", SCREEN_WIDTH/2 - 100, yOption, yesColor.r, yesColor.g, yesColor.b, TEXT_CENTER);
        textManager->drawText("NO",  SCREEN_WIDTH/2 + 100, yOption, noColor.r, noColor.g, noColor.b, TEXT_CENTER);
        return;
    }
    for(int i = 0; i < (int)PowerUp.size(); i++)
    {
        Entity* e = PowerUp[i];
        if (e->health > (FPS *2) || e->health % 12 < 6)
		{
		    if(e->texture == pointsTexture) CoinsAnimation->render(app.renderer, e->x, e->y);
			else blit(e->texture, e->x, e->y);
		}
    }

    if(player) {
        if(planet_y > SCREEN_HEIGHT) planet_y-=SCREEN_HEIGHT;
        player->planet->render(app.renderer,50 , planet_y);
        blit(player->texture, player->x, player->y);

        if(player->playerTail)
        {
            int tailX = player->x + (player->w / 2) - (player->playerTail->getFrameWidth() / 2) +2;
            int tailY = player->y + player->h- 3 ;
            player->playerTail->render(app.renderer, tailX, tailY);
        }
        if (player->hasShield && player->shieldAnimation) {
            float shieldX = player->x ;
            float shieldY = player->y ;
            player->shieldAnimation->render(app.renderer, shieldX, shieldY);
        }
    }

    for(int i = 0; i<(int)enemies.size(); i++)
    {
        Enemy* e = enemies[i];
        blit(e->texture, e->x, e->y);
    }
    for(int i = 0; i <(int)debris.size(); i++)
    {
        Debris* d = debris[i];
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
    for(int i = 0; i <(int)bullets.size(); i++)
    {
        Entity* e = bullets[i];
        if(e->side == SIDE_ALIEN)
        {
            BulletEnemyAnimation->render(app.renderer, e->x, e->y);
        }
        else  blit(e->texture, e->x, e->y);
    }
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);
    for(int i = 0; i < (int)explosions.size(); i++)
    {
        Explosion* e = explosions[i];
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(explosionTexture, e->a);
		blit(explosionTexture, e->x, e->y);
    }
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
    textManager->setFontSize(30);
    textManager->drawText("SCORE: "+ std::to_string(stage.score),SCREEN_WIDTH-10, 20, 255, 255, 255, TEXT_RIGHT);
    textManager->drawText( "WAVE: " + std::to_string(stage.wave),10, 20, 255, 255, 255, TEXT_LEFT);
    textManager->drawText("HIGHSCORE: "+ std::to_string(highscore),SCREEN_WIDTH - 10, 0, 0, 255, 0,TEXT_RIGHT);
    textManager->drawText("COIN: " + std::to_string(stage.coint), 10, 0, 255, 255, 255, TEXT_LEFT);
    if(player)
    {
      for (int i = 0; i < player->health; i++) {
            SDL_Rect heartRect = { 10 + i * (13 + 10), 50, 13, 13 };
            SDL_RenderCopy(app.renderer, lifeTexture, NULL, &heartRect);
        }
        if(player->hasShield)
            textManager->drawText("Time Shield: " + std::to_string((int)player->shieldTimer), 10, 60, 150, 150, 180, TEXT_LEFT);
    }
}
void cleanupStage() {
    if(stage.player)
    {
        delete stage.player;
        stage.player = nullptr;
    }
    if (BulletEnemyAnimation) {
        delete BulletEnemyAnimation;
        BulletEnemyAnimation = nullptr;
    }
    if (CoinsAnimation) {
        delete CoinsAnimation;
        CoinsAnimation = nullptr;
    }
    if (bulletUpgrade) {
        delete bulletUpgrade;
        bulletUpgrade = nullptr;
    }
}
