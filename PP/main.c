#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Include the menu and stage headers
#include "menu/menu_principale.h"
#include "stage .1/header.h"

#define SCREEN_W 1280
#define SCREEN_H 768

// Game states
typedef enum {
    STATE_MENU,
    STATE_STAGE_ONE,
    STATE_QUIT
} GameState;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    SDL_Surface *ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Jeu", NULL);

    GameState currentState = STATE_MENU;
    SDL_Event event;
    int quit = 0;

    // Initialize menu
    Menu menu;
    init_menu(&menu);

    // Initialize stage one variables
    Background bg1, bg2;
    int soloInit = 0, multiInit = 0;
    SDL_Surface *feuille1 = NULL, *feuille2 = NULL;
    SDL_Surface *etoileSurface = NULL;
    Feuille feuilles[NB_FEUILLes];
    Etoile etoiles[NB_ETOILES];
    Coin coins[NB_COINS];

    while (!quit) {
        switch (currentState) {
            case STATE_MENU: {
                // Handle menu state
                Afficher_menu(menu, ecran);
                SDL_Flip(ecran);

                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = 1;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) {
                                if (menu.selection == 5) {
                                    quit = 1;
                                } else if (menu.selection == 1) {
                                    // Start new game
                                    currentState = STATE_STAGE_ONE;
                                    
                                    // Initialize stage one resources
                                    feuille1 = IMG_Load("stage .1/feuille1.png");
                                    feuille2 = IMG_Load("stage .1/feuille2.png");
                                    etoileSurface = IMG_Load("stage .1/etoile.png");
                                    
                                    if (!feuille1 || !feuille2 || !etoileSurface) {
                                        printf("Error loading images: %s\n", IMG_GetError());
                                        quit = 1;
                                        break;
                                    }
                                    
                                    // Initialize leaves
                                    srand(time(NULL));
                                    for (int i = 0; i < NB_FEUILLes; i++) {
                                        int x = rand() % (1800 - feuille1->w);
                                        int y = -(rand() % 500);
                                        int vitesse = 2 + rand() % 4;
                                        SDL_Surface *imgChoisie = (rand() % 2 == 0) ? feuille1 : feuille2;
                                        initialiserFeuille(&feuilles[i], imgChoisie, x, y, vitesse);
                                    }

                                    // Initialize backgrounds
                                    initialiserBackground(&bg1, "stage .1/bg1.png", 0);
                                    initialiserBackground(&bg2, "stage .1/bg1.png", ecran->w / 2);
                                    
                                    // Initialize stars
                                    initialiserEtoiles(etoiles, NB_ETOILES, bg1.img);
                                    
                                    // Initialize coins
                                    SDL_Surface *argent = IMG_Load("stage .1/argent.png");
                                    if (!argent) {
                                        printf("Error loading argent.png: %s\n", IMG_GetError());
                                        quit = 1;
                                        break;
                                    }
                                    int posX[NB_COINS] = {600, 2250, 2450, 3000, 2800, 3100, 3850, 4400, 4900, 5450, 5680, 6200, 7310, 8100};
                                    int posY[NB_COINS] = {240, 655, 655, 240, 240, 720, 380, 775, 540, 620, 620, 275, 440, 390};
                                    for (int i = 0; i < NB_COINS; i++) {
                                        coins[i].img = argent;
                                        coins[i].pos.x = posX[i];
                                        coins[i].pos.y = posY[i];
                                        coins[i].pos.w = argent->w;
                                        coins[i].pos.h = argent->h;
                                    }
                                }
                            }
                            break;
                    }
                }
                miseajour(&menu);
                break;
            }

            case STATE_STAGE_ONE: {
                // Handle stage one state
                if (!soloInit) {
                    bg1.camera.x = 0;
                    bg1.camera.y = 150;
                    bg1.camera.w = ecran->w;
                    bg1.camera.h = ecran->h;
                    soloInit = 1;
                }

                // Handle stage one events
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = 1;
                    }
                    // Add stage one specific event handling here
                }

                // Update stage one
                const Uint8 *keys = SDL_GetKeyState(NULL);
                scrollingArrows(&bg1, keys);

                // Render stage one
                SDL_BlitSurface(bg1.img, &bg1.camera, ecran, &bg1.posEcran);
                
                // Animate and display leaves
                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuilles[i], 875, feuille1, feuille2);
                    afficherFeuille(feuilles[i], ecran);
                }

                // Animate and display stars
                float temps = (SDL_GetTicks() - SDL_GetTicks()) / 1000.0f;
                animerEtoiles(etoiles, NB_ETOILES, temps);
                afficherEtoiles(etoiles, NB_ETOILES, ecran, etoileSurface, &bg1.camera);

                // Display coins
                afficherCoins(coins, NB_COINS, ecran, &bg1.camera, bg1.posEcran.x);

                SDL_Flip(ecran);
                break;
            }
        }

        SDL_Delay(16);
    }

    // Cleanup
    SDL_FreeSurface(feuille1);
    SDL_FreeSurface(feuille2);
    SDL_FreeSurface(etoileSurface);
    SDL_FreeSurface(bg1.img);
    SDL_FreeSurface(bg2.img);
    SDL_FreeSurface(ecran);

    SDL_Quit();
    TTF_Quit();
    Mix_CloseAudio();

    return 0;
} 