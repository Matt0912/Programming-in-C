#ifndef __GLOBALSPRITERESOURCE_H__
#define __GLOBALSPRITERESOURCE_H__

#include "common.h"


#define NUM_SPRITES 50

using namespace std;


/* Sprite Resources */
enum {
    SR_SHIP_1,
    SR_BEAM_1,
    SR_BEAM_2,
    SR_BACKGROUND_1,
    SR_BACKGROUND_2,
    SR_BACKGROUND_3
};


class GlobalSpriteResource {
public:
    GlobalSpriteResource();
    ~GlobalSpriteResource();

    SDL_Surface* get(int num);
    void set(int num, SDL_Surface* sprite);
    void set(int num, string file);

    void draw(SDL_Surface* screen, int num,  int x, int y);
    void freemem();

    SDL_Surface** sprites;
};
#endif

