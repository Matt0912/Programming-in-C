#include "main.h"

int main ( int argc, char** argv ) {
    Game game = Game();
    if(game.init() > 0 ) {
        cout << "Error occured in Game initialization!" << endl;
        return 1;
    }
    game.gs = GS_MAIN;

    // program main loop
    while (game.gs != GS_DONE) {
        if(game.gs == GS_MAIN) {
            game.run();
        }
    } // end main loop

    printf("Exited cleanly\n");
    return 0;
}
