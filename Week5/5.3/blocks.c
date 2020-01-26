#include <stdio.h>
#include <stdlib.h>
#include "neillsdl2.h"

#define RECTSIZE 20
#define MILLISECONDDELAY 20

typedef enum bool {false, true} bool;
void keyboard(bool KEYS[1000], SDL_Simplewin *sw);

int main(void)
{
   bool KEYS[1000];
   SDL_Simplewin sw;
   SDL_Rect rectangle;
   int i;
   rectangle.w = RECTSIZE;
   rectangle.h = RECTSIZE;

   for(i = 0; i < 322; i++) {
     KEYS[i] = false;
   }

   Neill_SDL_Init(&sw);
   do{

      SDL_Delay(MILLISECONDDELAY);
      keyboard(KEYS, &sw);

      /* Choose a random colour, a mixture of red, green and blue. */
      Neill_SDL_SetDrawColour(&sw, rand()%SDL_8BITCOLOUR,
                                   rand()%SDL_8BITCOLOUR,
                                   rand()%SDL_8BITCOLOUR);

      /* Filled Rectangle, fixed size, random position */
      if(KEYS[SDLK_h]) {
        rectangle.x = rand()%(WWIDTH-RECTSIZE);
        rectangle.y = rand()%(WHEIGHT-RECTSIZE);
        SDL_RenderFillRect(sw.renderer, &rectangle);
      }

      /* Unfilled Rectangle, fixed size, random position */
      if(KEYS[SDLK_r]) {
        rectangle.x = rand()%(WWIDTH-RECTSIZE);
        rectangle.y = rand()%(WHEIGHT-RECTSIZE);
        SDL_RenderDrawRect(sw.renderer, &rectangle);
      }

      Neill_SDL_UpdateScreen(&sw);

      /* Has anyone pressed ESC or killed the SDL window ?
         Must be called frequently - it's the only way of escaping void SDL_KeyInput() */



   }while(!sw.finished);

   /* Clear up graphics subsystems */
   SDL_Quit();
   atexit(SDL_Quit);

   return 0;

}



void keyboard(bool KEYS[1000], SDL_Simplewin *sw) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym==SDLK_ESCAPE) {
                  sw->finished = 1;
                  break;
                }
                KEYS[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                KEYS[event.key.keysym.sym] = false;
                break;
            default:
                break;
          }
      }
}
