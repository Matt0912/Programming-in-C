#ifndef __BULLET_H__
#define __BULLET_H__

#include "common.h"

using namespace std;

enum {
    FLIGHT_NORMAL = 0,
    FLIGHT_SINE,
    FLIGH_HOMING
};

class Bullet {
public:
    Bullet();
    ~Bullet();
    void draw(SDL_Surface* screen, int x, int y);

    bool exists;
    int x;
    int y;
    int speedX;
    int speedY;
    int damage;
    int flightType;

    SDL_Surface* image;
};
#endif
