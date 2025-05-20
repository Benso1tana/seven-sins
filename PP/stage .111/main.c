#include "header.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_EnableKeyRepeat(10, 10);

    SDL_Surface *ecran = SDL_SetVideoMode(1800, 875, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Partage d'\u00e9cran - SDL", NULL);
    TTF_Font *police = TTF_OpenFont("DejaVuSans.ttf", 24);

    EtatJeu etatActuel = MENU_PRINCIPAL;
    SDL_Event event;

    Uint32 debut = SDL_GetTicks();

    Background bg1, bg2;
    int soloInit = 0, multiInit = 0;

    const Uint8 *keys = SDL_GetKeyState(NULL);

    SDL_Surface *feuille1 = IMG_Load("feuille1.png");
    SDL_Surface *feuille2 = IMG_Load("feuille2.png");

    if (!feuille1 || !feuille2) {
        printf("Erreur de chargement des images de feuilles\n");
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Surface *etoileSurface = IMG_Load("etoile.png");
    if (!etoileSurface) {
        printf("Erreur de chargement de l'image d'\u00e9toile\n");
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    const int BG1_WIDTH = ecran->w / 2;

    Feuille feuilles[NB_FEUILLes];
    srand(time(NULL));

    for (int i = 0; i < NB_FEUILLes; i++) {
        int x = rand() % (1800 - 50);
        int y = rand() % (-200);
        int vitesse = 1 + rand() % 3;
        SDL_Surface *imgChoisie = (rand() % 2 == 0) ? feuille1 : feuille2;

        initialiserFeuille(&feuilles[i], imgChoisie, x, y, vitesse);
    }

    initialiserBackground(&bg1, "bg1.png", 0);
    bg1.camera.x = 0;
    bg1.camera.y = 150;
    bg1.camera.w = ecran->w / 2;
    bg1.camera.h = ecran->h;
    bg1.posEcran.x = 0;
    bg1.posEcran.y = 0;

    initialiserBackground(&bg2, "bg1.png", ecran->w / 2);
    bg2.camera.x = 0;
    bg2.camera.y = 150; // Positionne bg2 un peu plus bas
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
                    if (feuilles[i].pos.x + feuilles[i].img->w <= bg1.camera.w &&
                        feuilles[i].pos.y + feuilles[i].img->h <= bg1.camera.h &&
                        feuilles[i].pos.x >= 0 && feuilles[i].pos.y >= 0) {
                        afficherFeuille(feuilles[i], ecran);
                    }
                }

                SDL_BlitSurface(bg2.img, &bg2.camera, ecran, &bg2.posEcran);

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
                    if (feuilles[i].pos.x + feuilles[i].img->w <= bg2.camera.x + bg2.camera.w &&
                        feuilles[i].pos.x >= bg2.camera.x &&
                        feuilles[i].pos.y >= bg2.camera.y &&
                        feuilles[i].pos.y + feuilles[i].img->h <= bg2.camera.y + bg2.camera.h) {
                        SDL_Rect dst = {
                            feuilles[i].pos.x - bg2.camera.x + bg2.posEcran.x,
                            feuilles[i].pos.y - bg2.camera.y,
                            0, 0
                        };
                        SDL_BlitSurface(feuilles[i].img, NULL, ecran, &dst);
                    }
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
    SDL_Quit();
    TTF_Quit();

    return 0;
}
