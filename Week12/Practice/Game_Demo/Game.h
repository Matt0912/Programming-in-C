#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "GlobalSpriteResource.h"
#include "BulletFlight.h"

#include "Player.h"

using namespace std;

enum {
    GS_TITLESCREEN,
    GS_MAIN,
    GS_PAUSE,
    GS_DONE
};

class Game {
public:
    Game();
    ~Game();

    int init();
    void run();
    void reset();

    void backGround();
    void handleInput();
    void keyboard();

    void handleBullets();
    int getEmptyBullet();
    void shootbeam(int beam);

    Player* player;

    SDL_Surface* screen;

    int gs;

    bool    leftPressed,
            rightPressed,
            upPressed,
            downPressed,
            spacePressed,
            aPressed,
            sPressed,
            dPressed,
            wPressed;

    GlobalSpriteResource* gsr;
    BulletFlight* bf;

    int levelY;
    int levelScrollSpeed;
    int backgroundImage;

    int gameSpeed;

};
#endif
