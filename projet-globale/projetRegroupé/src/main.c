#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "menu/menu_principale.h"
#include "stages/stage1/header.h"
#include "stages/stage2/header.h"
#include "stages/stage_finale/header.h"
#include "player/player.h"
#include "story/histoire.h"

// Forward declarations for stage-specific functions
void initialiserBackgroundStage1(Background *bg, const char *path, int xPosEcran);
void afficherBackgroundStage1(Background bg, SDL_Surface *ecran);
void scrollingKeysStage1(Background *bg, const Uint8 *keys);

void initialiserBackgroundStage2(Background *bg, const char *path, int xPosEcran);
void afficherBackgroundStage2(Background bg, SDL_Surface *ecran);

void initialiserBackgroundStageFinale(Background *bg, const char *path, int xPosEcran);
void afficherBackgroundStageFinale(Background bg, SDL_Surface *ecran);
void scrollingKeysStageFinale(Background *bg, const Uint8 *keys);

#define SCREEN_W 1280
#define SCREEN_H 768

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_OPTIONS,
    GAME_STATE_STAGE1,
    GAME_STATE_STAGE2,
    GAME_STATE_STAGE_FINALE,
    GAME_STATE_STORY,
    GAME_STATE_QUIT
} GameState;

int main(int argc, char *argv[]) {
    // Initialize SDL with error checking
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Surface *ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_WM_SetCaption("Seven Sins", NULL);

    GameState currentState = GAME_STATE_MENU;
    SDL_Event event;
    int running = 1;

    // Initialize menu
    Menu menu;
    init_menu(&menu);

    // Initialize stage resources
    Background bg;
    bg.img = NULL;  // Initialize to NULL
    bg.posEcran.x = 0;
    bg.posEcran.y = 0;
    bg.camera.x = 0;
    bg.camera.y = 0;
    bg.camera.w = SCREEN_W;
    bg.camera.h = SCREEN_H;

    Player* player = NULL;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
                break;
            }

            switch (currentState) {
                case GAME_STATE_MENU:
                    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                        // Check which button was clicked
                        if (selectionner_bouton(menu.pos_jouer1)) {
                            currentState = GAME_STATE_STAGE1;
                            // Initialize stage 1 background
                            if (bg.img) {
                                SDL_FreeSurface(bg.img);
                                bg.img = NULL;
                            }
                            initialiserBackgroundStage1(&bg, "assets/images/backgrounds/bg1.png", 0);
                            if (!bg.img) {
                                printf("Failed to load background image: %s\n", SDL_GetError());
                                currentState = GAME_STATE_MENU;
                            } else {
                                // Initialize leaves for stage 1
                                initialiserFeuilles();
                                if (player) destroyPlayer(player);
                                player = createPlayer(100, 100); // Set start position as needed
                            }
                        }
                        else if (selectionner_bouton(menu.pos_option1)) {
                            currentState = GAME_STATE_OPTIONS;
                        }
                        else if (selectionner_bouton(menu.pos_quitter1)) {
                            currentState = GAME_STATE_QUIT;
                            running = 0;
                        }
                        else if (selectionner_bouton(menu.pos_stage2)) {
                            currentState = GAME_STATE_STAGE2;
                            // Initialize stage 2 background
                            if (bg.img) {
                                SDL_FreeSurface(bg.img);
                                bg.img = NULL;
                            }
                            initialiserBackgroundStage2(&bg, "assets/images/backgrounds/bg2.png", 0);
                            if (!bg.img) {
                                printf("Failed to load background image: %s\n", SDL_GetError());
                                currentState = GAME_STATE_MENU;
                            }
                        }
                        else if (selectionner_bouton(menu.pos_historique1)) {
                            currentState = GAME_STATE_STORY;
                        }
                    }
                    break;

                case GAME_STATE_OPTIONS:
                    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                        if (selectionner_bouton(menu.pos_retour)) {
                            currentState = GAME_STATE_MENU;
                        }
                        else if (selectionner_bouton(menu.pos_augmenter)) {
                            menu.volume_music += 10;
                            if (menu.volume_music > 100) menu.volume_music = 100;
                            Mix_VolumeMusic(menu.volume_music);
                        }
                        else if (selectionner_bouton(menu.pos_diminuer)) {
                            menu.volume_music -= 10;
                            if (menu.volume_music < 0) menu.volume_music = 0;
                            Mix_VolumeMusic(menu.volume_music);
                        }
                        else if (selectionner_bouton(menu.pos_plein)) {
                            if (menu.etat_plein_ecran == 0) {
                                SDL_Surface *newScreen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                                if (newScreen) {
                                    ecran = newScreen;
                                    menu.etat_plein_ecran = 1;
                                }
                            }
                        }
                        else if (selectionner_bouton(menu.pos_normale)) {
                            if (menu.etat_plein_ecran == 1) {
                                SDL_Surface *newScreen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                                if (newScreen) {
                                    ecran = newScreen;
                                    menu.etat_plein_ecran = 0;
                                }
                            }
                        }
                    }
                    break;

                case GAME_STATE_STAGE1:
                    if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = GAME_STATE_MENU;
                        }
                        if (player) handlePlayerInput(player, SDL_GetKeyState(NULL), NULL);
                        const Uint8 *keys = SDL_GetKeyState(NULL);
                        if (bg.img) {
                            scrollingKeysStage1(&bg, keys);
                        }
                    }
                    break;

                case GAME_STATE_STAGE2:
                    if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = GAME_STATE_MENU;
                        }
                        // Handle stage 2 movement
                        const Uint8 *keys = SDL_GetKeyState(NULL);
                        if (bg.img) {  // Only scroll if background is loaded
                            scrollingKeysStageFinale(&bg, keys);
                        }
                    }
                    break;

                case GAME_STATE_STAGE_FINALE:
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        currentState = GAME_STATE_MENU;
                    }
                    break;

                case GAME_STATE_STORY:
                    afficher_histoire(ecran);
                    currentState = GAME_STATE_MENU;
                    break;

                case GAME_STATE_QUIT:
                    running = 0;
                    break;
            }
        }

        // Update menu selection based on mouse position
        miseajour(&menu);

        // Update player in stage 1
        if (currentState == GAME_STATE_STAGE1 && player) {
            updatePlayer(player);
        }

        // Clear screen
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

        // Draw current state
        switch (currentState) {
            case GAME_STATE_MENU:
                Afficher_menu(menu, ecran);
                break;

            case GAME_STATE_OPTIONS:
                Afficher_menu_option(menu, ecran);
                break;

            case GAME_STATE_STAGE1:
                if (bg.img) {
                    afficherBackgroundStage1(bg, ecran);
                    if (player) renderPlayer(player, ecran);
                }
                break;

            case GAME_STATE_STAGE2:
                if (bg.img) {  // Only draw if background is loaded
                    afficherBackgroundStage2(bg, ecran);
                }
                break;

            case GAME_STATE_STAGE_FINALE:
                if (bg.img) {  // Only draw if background is loaded
                    afficherBackgroundStageFinale(bg, ecran);
                }
                break;

            case GAME_STATE_STORY:
                // Nothing to draw in story state
                break;

            case GAME_STATE_QUIT:
                // Nothing to draw in quit state
                break;
        }

        SDL_Flip(ecran);
    }

    // Cleanup
    if (player) destroyPlayer(player);
    if (bg.img) {
        SDL_FreeSurface(bg.img);
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
} 