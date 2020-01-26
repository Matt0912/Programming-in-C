#include "Player.h"


Player::Player() {
    // load an image
    image = NULL;
    if (!image) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    } else {
        SDL_SetColorKey(image, SDL_SRCCOLORKEY, 0xFF00FF);
        printf("loaded image\n");
    }

    for(int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].exists = false;
    }

}

Player::~Player() {
    // free loaded bitmap
    if(image != NULL) {
        SDL_FreeSurface(image);
    }
}


void Player::draw(SDL_Surface* screen, int x, int y) {
    if(image == NULL) {
        cout << "image null" << endl;
        return;
    }
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    SDL_BlitSurface(image, 0, screen, &dstrect);
}
