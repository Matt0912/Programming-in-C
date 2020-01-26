#include "GlobalSpriteResource.h"

GlobalSpriteResource::GlobalSpriteResource() {
    sprites = new SDL_Surface*[NUM_SPRITES];
}

GlobalSpriteResource::~GlobalSpriteResource() {
    freemem();
}

void GlobalSpriteResource::set(int num, SDL_Surface* surface) {
    sprites[num] = surface;
}

void GlobalSpriteResource::set(int num, string file) {
     sprites[num] = SDL_LoadBMP(file.c_str());
    if (!sprites[num]) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    } else {
        SDL_SetColorKey(sprites[num], SDL_SRCCOLORKEY, 0xFF00FF);
        printf("loaded sprite\n");
    }
}

SDL_Surface* GlobalSpriteResource::get(int num) {
    if(num >= 0 && num < NUM_SPRITES) {
        return sprites[num];
    }
    return NULL;
}


void GlobalSpriteResource::draw(SDL_Surface* screen, int num,  int x, int y) {
    if(sprites[num] == NULL) {
        cout << "image null" << endl;
        return;
    }
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    SDL_BlitSurface(sprites[num], 0, screen, &dstrect);
}


void GlobalSpriteResource::freemem() {
    for(int i = 0; i < NUM_SPRITES; i++) {
        if(sprites[i] != NULL) {
            SDL_free(sprites[i]);
        }
    }
}
