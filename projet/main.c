#include "game.h"
#include <stdio.h>
#include <stdlib.h>

// --- Définissez le nom du port série ici ---
// --- !! AJUSTEZ CECI À VOTRE SYSTÈME !! ---
#ifdef _WIN32
    // Exemple pour Windows (peut être COM1, COM3, COM4, etc.)
    const char *SERIAL_PORT = "COM3";
#else
    // Exemple pour Linux : remplacez par le vôtre ! (ex: /dev/ttyACM0, /dev/ttyUSB0)
    const char *SERIAL_PORT = "/dev/ttyACM0";
    // Exemple pour macOS (peut être /dev/tty.usbmodemXXXX, etc.)
    // const char *SERIAL_PORT = "/dev/tty.usbmodem14201";
#endif


int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    printf("Starting Game...\n");
    Game game;

    // Passer le nom du port série à init_game
    // La signature de init_game a été restaurée pour accepter le port
    if (!init_game(&game, 1, SERIAL_PORT)) {
        fprintf(stderr, "Game initialization failed!\n");
        return EXIT_FAILURE;
    }

    printf("Entering Main Loop...\n");
    while (game.running) {
        switch(game.game_state) {
            case STATE_RUNNING:
            case STATE_PAUSED:
            case STATE_GAMEOVER:
                handle_input(&game);
                 if (game.game_state == STATE_RUNNING) {
                     update_game(&game);
                 }
                render_game(&game);
                break;
            case STATE_PUZZLE:
                {
                    PuzzleState puzzle_result = run_puzzle(&game);
                    if (puzzle_result == PUZZLE_STATE_WON) {
                        player_gain_life(&game.player);
                    } else {
                        printf("Puzzle Lost or Exited.\n");
                    }
                    if (game.game_state == STATE_PUZZLE) { // Safety
                        game.game_state = STATE_RUNNING;
                    }
                }
                break;
        }
        SDL_Delay(16);
    }
    printf("Exiting Main Loop.\n");
    cleanup_game(&game);
    printf("Game Exited Gracefully.\n");
    return EXIT_SUCCESS;
}
