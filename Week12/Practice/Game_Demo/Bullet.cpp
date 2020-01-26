#include "Bullet.h"

Bullet::Bullet() {
    this->exists = 0;
}


Bullet::~Bullet() {

}

void Bullet::draw(SDL_Surface* screen, int x, int y) {
    if(image == NULL) {
        cout << "image null" << endl;
        return;
    }
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    SDL_BlitSurface(image, 0, screen, &dstrect);
}
