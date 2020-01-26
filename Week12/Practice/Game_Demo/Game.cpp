#include "Game.h"

Game::Game() {

}

Game::~Game() {

}

int Game::init() {
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen ) {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    SDL_EnableKeyRepeat(0,0);

    gsr = new GlobalSpriteResource();
    gsr->set(SR_SHIP_1, "sprites/spaceship.bmp");
    gsr->set(SR_BEAM_1, "sprites/beam_1.bmp");
    gsr->set(SR_BEAM_2, "sprites/beam_2.bmp");
    gsr->set(SR_BACKGROUND_1, "sprites/back_1.bmp");
    gsr->set(SR_BACKGROUND_2, "sprites/back_2.bmp");

    bf = new BulletFlight();

    player = new Player();

    reset();
//    SDL_TimerID bullet_timer = SDL_AddTimer(100, &handleBullets, void);

    return 0;
}

void Game::run() {
    keyboard();
    handleInput();
    // clear screen
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

    backGround();

    handleBullets();

    player->draw(screen, player->x, player->y);
    // finally, update the screen :)
    SDL_Flip(screen);

}

void Game::reset() {
    leftPressed     = false;
    rightPressed    = false;
    upPressed       = false;
    downPressed     = false;
    spacePressed    = false;
    aPressed        = false;
    sPressed        = false;
    dPressed        = false;
    wPressed        = false;

    player->image = gsr->get(SR_SHIP_1);
    player->x = 320;
    player->y = 440;
    player->speed = 9;
    player->beam = PLAYER_BEAM_NORMAL;
    player->lastShot = SDL_GetTicks();
    player->shotDelay = 200;


    levelY = 0;
    levelScrollSpeed = 2;
    backgroundImage = SR_BACKGROUND_2;

    gameSpeed = 30;
}

void Game::backGround() {
    // draw background
    for(int y = -gsr->get(backgroundImage)->h; y <= screen->h; y += gsr->get(backgroundImage)->h) {
        for(int x = 0; x <= screen->w; x += gsr->get(backgroundImage)->w) {
            //   cout << "X " << x << " Y " << y << " LevelY " << levelY << endl;
            gsr->draw(screen, backgroundImage, x, y + levelY);
        }
    }

    levelY += levelScrollSpeed;
    if(levelY > gsr->get(backgroundImage)->h) {
        levelY = 0;
    }
}

void Game::handleInput() {
    if(leftPressed) {
        if(player->x - player->speed >= 0) {
            player->x -= player->speed;
        }
    }
    if(rightPressed) {
        if(player->x + player->speed <= screen->w) {
            player->x += player->speed;
        }
    }
    if(upPressed) {
        if(player->y - player->speed >= 0) {
            player->y -= player->speed;
        }
    }
    if(downPressed) {
        if(player->y + player->speed <= screen->h) {
            player->y += player->speed;
        }
    }
    if(sPressed) {
        if(SDL_GetTicks() - player->lastShot > player->shotDelay) {
            shootbeam(player->beam);
        }
    }
}

void Game::keyboard() {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // check for messages
            switch (event.type) {
                // exit if the window is closed
            case SDL_QUIT:
                gs = GS_DONE;
                break;
                // check for keypresses
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        leftPressed = true;
                        break;
                    case SDLK_RIGHT:
                        rightPressed = true;
                        break;
                    case SDLK_DOWN:
                        downPressed = true;
                        break;
                    case SDLK_UP:
                        upPressed = true;
                        break;
                    case SDLK_SPACE:
                        spacePressed = true;
                        break;
                    case SDLK_a:
                        aPressed = true;
                        break;
                     case SDLK_s:
                        sPressed = true;
                        break;
                    case SDLK_d:
                        dPressed = true;
                        break;
                    case SDLK_w:
                        wPressed = true;
                        break;
                    case SDLK_ESCAPE:
                        gs = GS_DONE;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        leftPressed = false;
                        break;
                    case SDLK_RIGHT:
                        rightPressed = false;
                        break;
                    case SDLK_DOWN:
                        downPressed = false;
                        break;
                    case SDLK_UP:
                        upPressed = false;
                        break;
                    case SDLK_SPACE:
                        spacePressed = false;
                        break;
                    case SDLK_a:
                        aPressed = false;
                        break;
                     case SDLK_s:
                        sPressed = false;
                        break;
                    case SDLK_d:
                        dPressed = false;
                        break;
                    case SDLK_w:
                        wPressed = false;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
			}
        } // end of message processing
}


void Game::handleBullets() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(player->bullets[i].exists) {
       //     cout << "Draw Bullet" << endl;
         //   cout << "Bullet Y " << player->bullets[i].y << endl;
            bf->process(player->bullets[i]);

            player->bullets[i].draw(screen, player->bullets[i].x, player->bullets[i].y);
            if(player->bullets[i].y < -50 || player->bullets[i].y > screen->h +50 ||
                    player->bullets[i].x < -50 || player->bullets[i].x > screen->w +50) {
                player->bullets[i].exists = false;
            }
        }
    }
}

int Game::getEmptyBullet() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(!player->bullets[i].exists) {
            return i;
        }
    }
    return -1;
}

void Game::shootbeam(int beam) {
    if(beam == PLAYER_BEAM_NORMAL) {
        int i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -10;
                player->bullets[i].speedX = 0;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }

        i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -sqrt(91);
                player->bullets[i].speedX = -3;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }
        i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -sqrt(91);
                player->bullets[i].speedX = 3;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }

        i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -sqrt(75);
                player->bullets[i].speedX = -5;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }
        i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -sqrt(75);
                player->bullets[i].speedX = 5;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }

        i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -sqrt(51);
                player->bullets[i].speedX = 7;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }
        i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_1);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 20;
                player->bullets[i].speedY = -sqrt(51);
                player->bullets[i].speedX = -7;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }
    } else if(beam == PLAYER_BEAM_CHARGE) {
        int i = getEmptyBullet();
        if(i != -1) {
                //   cout << "Add bullet to index " << i << endl;
                player->bullets[i].exists = true;
                player->lastShot = SDL_GetTicks();

                player->bullets[i].image = gsr->get(SR_BEAM_2);
                player->bullets[i].x = player->x + player->image->w/2 - player->bullets[i].image->w/2;
                player->bullets[i].y = player->y - 30;
                player->bullets[i].speedY = -8;
                player->bullets[i].speedX = 0;
                player->bullets[i].damage = 1;
                player->bullets[i].flightType = BULLET_FLIGHT_NORMAL;
        }
    }

}
