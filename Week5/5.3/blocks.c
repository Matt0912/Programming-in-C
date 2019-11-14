#include <stdio.h>
#include <stdlib.h>
#include "neillsdl2.h"

#define RECTSIZE 300
#define MILLISECONDDELAY 20
#define SIZE 3

int main(void)
{
   int x, y, i;
   SDL_Simplewin sw;
   SDL_Rect rectangle;
   char userInput[] = "1243 5678";
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   rectangle.w = RECTSIZE;
   rectangle.h = RECTSIZE;

   Neill_SDL_ReadFont(fontdata, "mode7.fnt");

   Neill_SDL_Init(&sw);
   do{
      SDL_Delay(MILLISECONDDELAY);

      /* Choose a random colour, a mixture of red, green and blue. */
      Neill_SDL_SetDrawColour(&sw, 255,255,255);

      /* Filled Rectangle, fixed size, random position
      rectangle.x = rand()%(WWIDTH-RECTSIZE);
      rectangle.y = rand()%(WHEIGHT-RECTSIZE);
      SDL_RenderFillRect(sw.renderer, &rectangle); */

      /* Unfilled Rectangle, fixed size, random position */
      for (y = 0; y < SIZE; y++) {
        for (x = 0; x < SIZE; x++) {
          rectangle.x = RECTSIZE * x;
          rectangle.y = RECTSIZE * y;
          SDL_RenderDrawRect(sw.renderer, &rectangle);
        }
      }

      i = 0;
      while (userInput[i]){
        for (y = 0; y < SIZE; y++) {
          for (x = 0; x < SIZE; x++) {
            Neill_SDL_DrawChar(&sw, fontdata, userInput[i],
              RECTSIZE/2 + RECTSIZE * x, RECTSIZE/2 + RECTSIZE * y);
            i++;
          }
        }
      }





      Neill_SDL_UpdateScreen(&sw);

      /* Has anyone pressed ESC or killed the SDL window ?
         Must be called frequently - it's the only way of escaping  */
      Neill_SDL_Events(&sw);

   }while(!sw.finished);

   /* Clear up graphics subsystems */
   SDL_Quit();
   atexit(SDL_Quit);

   return 0;

}
