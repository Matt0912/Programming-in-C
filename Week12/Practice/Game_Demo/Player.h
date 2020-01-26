#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"
#include "Bullet.h"

#define MAX_BULLETS 120

using namespace std;

enum {
    PLAYER_BEAM_NORMAL,
    PLAYER_BEAM_CHARGE
};

class Player {
public:
    Player();
    ~Player();
    void draw(SDL_Surface* screen, int x, int y);

    float x;
    float y;
    int speed;
    SDL_Surface* image;

    int beam;
    long lastShot;
    int shotDelay;

    Bullet bullets[MAX_BULLETS];
};
#endif
