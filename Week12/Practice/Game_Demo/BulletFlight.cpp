#include "BulletFlight.h"

BulletFlight::BulletFlight() {

}

BulletFlight::~BulletFlight() {

}

void BulletFlight::process(Bullet &bullet) {
    if(bullet.flightType == BULLET_FLIGHT_NORMAL) {
      //  cout << "Bullet Y, Bullet Speed " << bullet.y << " " << bullet.speed << endl;
        bullet.y += bullet.speedY;
        bullet.x += bullet.speedX;
     //   cout << "Bullet Y new " << bullet.y << endl;
    } else if(bullet.flightType == BULLET_FLIGHT_SINE) {
      //  cout << "Bullet Y, Bullet Speed " << bullet.y << " " << bullet.speed << endl;
        int origX = bullet.x;
        bullet.x = origX + 20*sin(bullet.y*1.8);
        bullet.y += bullet.speedY;
     //   cout << "Bullet Y new " << bullet.y << endl;
    }
}
