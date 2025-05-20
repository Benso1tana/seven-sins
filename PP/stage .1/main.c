#include "header.h"
#include <time.h>
#include <stdlib.h>

Coin coins[NB_COINS];

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_EnableKeyRepeat(10, 10);

    SDL_Surface *ecran = SDL_SetVideoMode(1800, 875, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Partage d'écran - SDL", NULL);
    TTF_Font *police = TTF_OpenFont("DejaVuSans.ttf", 24);

    EtatJeu etatActuel = MENU_PRINCIPAL;
    SDL_Event event;

    Uint32 debut = SDL_GetTicks();

    Background bg1, bg2;
    int soloInit = 0, multiInit = 0;

    const Uint8 *keys = SDL_GetKeyState(NULL);

    SDL_Surface *feuille1 = IMG_Load("feuille1.png");
    if (!feuille1) {
        printf("Erreur de chargement de feuille1.png: %s\n", IMG_GetError());
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Surface *feuille2 = IMG_Load("feuille2.png");
    if (!feuille2) {
        printf("Erreur de chargement de feuille2.png: %s\n", IMG_GetError());
        SDL_FreeSurface(feuille1);
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Surface *etoileSurface = IMG_Load("etoile.png");
    if (!etoileSurface) {
        printf("Erreur de chargement de l'image d'étoile\n");
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Surface *argent = IMG_Load("argent.png");
    if (!argent) {
        printf("Erreur de chargement de l'image argent\n");
        SDL_Quit();
        TTF_Quit();
        return 1;
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

    Feuille feuilles[NB_FEUILLes];
    srand(time(NULL));

    for (int i = 0; i < NB_FEUILLes; i++) {
        int x = rand() % (1800 - feuille1->w);
        int y = -(rand() % 500);
        int vitesse = 2 + rand() % 4;
        SDL_Surface *imgChoisie = (rand() % 2 == 0) ? feuille1 : feuille2;

        initialiserFeuille(&feuilles[i], imgChoisie, x, y, vitesse);
    }
    int largeurTotale = 8801;
    int largeurMoitie = largeurTotale / 2;

    initialiserBackground(&bg1, "bg1.png", 0);
    if (!bg1.img) {
        printf("Erreur de chargement du background stage 1\n");
        SDL_FreeSurface(feuille1);
        SDL_FreeSurface(feuille2);
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    bg1.camera.x = 0;
    bg1.camera.y = 150;
    bg1.camera.w = ecran->w;
    bg1.camera.h = ecran->h;
    bg1.posEcran.x = 0;
    bg1.posEcran.y = 0;

    initialiserBackground(&bg2, "bg1.png", ecran->w / 2);
    if (!bg2.img) {
        printf("Erreur de chargement du background stage 1 (partie 2)\n");
        SDL_FreeSurface(feuille1);
        SDL_FreeSurface(feuille2);
        SDL_FreeSurface(bg1.img);
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    bg2.camera.x = 0;
    bg2.camera.y = 150;
    bg2.camera.w = ecran->w / 2;
    bg2.camera.h = ecran->h;
    bg2.posEcran.x = ecran->w / 2;
    bg2.posEcran.y = 0;

    Etoile etoiles[NB_ETOILES];
    initialiserEtoiles(etoiles, NB_ETOILES, bg1.img);

    while (etatActuel != QUITTER) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            etatActuel = QUITTER;

        switch (etatActuel) {
            case MENU_PRINCIPAL:
                afficherGuide(ecran, "Choisissez votre mode : 1 - Solo, 2 - Multi", police);
                SDL_Flip(ecran);
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_1) {
                        etatActuel = MODE_SOLO;
                        soloInit = 0;
                    } else if (event.key.keysym.sym == SDLK_2) {
                        etatActuel = MODE_MULTI;
                        multiInit = 0;
                    }
                }
                break;

            case MODE_SOLO: {
                if (!soloInit) {
                    bg1.camera.x = 0;
                    bg1.camera.y = 150;
                    bg1.camera.w = ecran->w;
                    bg1.camera.h = ecran->h;
                    soloInit = 1;
                }

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
                    etatActuel = MODE_MULTI;
                    multiInit = 0;
                    break;
                }

                scrollingArrows(&bg1, keys);
                SDL_BlitSurface(bg1.img, &bg1.camera, ecran, &bg1.posEcran);

                // Affiche coins dans bg1
                afficherCoins(coins, NB_COINS, ecran, &bg1.camera, bg1.posEcran.x);

                float temps = (SDL_GetTicks() - debut) / 1000.0f;
                animerEtoiles(etoiles, NB_ETOILES, temps);

                for (int i = 0; i < NB_ETOILES; i++) {
                    SDL_Rect dst = etoiles[i].pos;
                    dst.x -= bg1.camera.x;
                    dst.y -= bg1.camera.y;

                    Uint8 alpha = (Uint8)etoiles[i].luminosite;
                    SDL_SetAlpha(etoileSurface, SDL_SRCALPHA, alpha);
                    SDL_BlitSurface(etoileSurface, NULL, ecran, &dst);
                }

                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuilles[i], 875, feuille1, feuille2);
                    afficherFeuille(feuilles[i], ecran);
                }

                afficherTemps(ecran, debut, police);
                afficherGuide(ecran, "Appuyez sur 2 pour passer en mode multi", police);
                SDL_Flip(ecran);
                break;
            }

            case MODE_MULTI: {
                if (!multiInit) {
                    bg1.camera.w = ecran->w / 2;
                    bg1.camera.h = ecran->h;
                    bg2.camera.w = ecran->w / 2;
                    bg2.camera.h = ecran->h;
                    multiInit = 1;
                }

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                    etatActuel = MODE_SOLO;
                    soloInit = 0;
                    break;
                }

                scrollingArrows(&bg1, keys);
                scrollingKeys(&bg2, keys);

                SDL_BlitSurface(bg1.img, &bg1.camera, ecran, &bg1.posEcran);

                // Affiche coins dans bg1 (gauche)
                afficherCoins(coins, NB_COINS, ecran, &bg1.camera, bg1.posEcran.x);

                float temps = (SDL_GetTicks() - debut) / 1000.0f;
                animerEtoiles(etoiles, NB_ETOILES, temps);

                for (int i = 0; i < NB_ETOILES; i++) {
                    int x = etoiles[i].pos.x - bg1.camera.x;
                    int y = etoiles[i].pos.y - bg1.camera.y;
                    if (x >= 0 && x + etoiles[i].pos.w <= bg1.camera.w && y >= 0 && y + etoiles[i].pos.h <= bg1.camera.h) {
                        Uint8 alpha = (Uint8)etoiles[i].luminosite;
                        SDL_SetAlpha(etoileSurface, SDL_SRCALPHA, alpha);
                        SDL_Rect dst = {x, y, 0, 0};
                        SDL_BlitSurface(etoileSurface, NULL, ecran, &dst);
                    }
                }

                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuilles[i], 875, feuille1, feuille2);
                    afficherFeuille(feuilles[i], ecran);
                }

                SDL_BlitSurface(bg2.img, &bg2.camera, ecran, &bg2.posEcran);

                // Affiche coins dans bg2 (droite)
                afficherCoins(coins, NB_COINS, ecran, &bg2.camera, bg2.posEcran.x);

                for (int i = 0; i < NB_ETOILES; i++) {
                    int x = etoiles[i].pos.x - bg2.camera.x;
                    int y = etoiles[i].pos.y - bg2.camera.y;
                    if (x >= 0 && x + etoiles[i].pos.w <= bg2.camera.w && y >= 0 && y + etoiles[i].pos.h <= bg2.camera.h) {
                        Uint8 alpha = (Uint8)etoiles[i].luminosite;
                        SDL_SetAlpha(etoileSurface, SDL_SRCALPHA, alpha);
                        SDL_Rect dst = {x + bg2.posEcran.x, y, 0, 0};
                        SDL_BlitSurface(etoileSurface, NULL, ecran, &dst);
                    }
                }

                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuilles[i], 875, feuille1, feuille2);
                    afficherFeuille(feuilles[i], ecran);
                }

                afficherTemps(ecran, debut, police);
                afficherGuide(ecran, "Appuyez sur R pour revenir en mode solo", police);
                SDL_Flip(ecran);
                break;
            }

            default:
                break;
        }

        SDL_Delay(16);
    }

    SDL_FreeSurface(feuille1);
    SDL_FreeSurface(feuille2);
    SDL_FreeSurface(etoileSurface);
    SDL_FreeSurface(bg1.img);
    SDL_FreeSurface(bg2.img);
    SDL_FreeSurface(argent);

    SDL_Quit();
    TTF_Quit();

    return 0;
}

