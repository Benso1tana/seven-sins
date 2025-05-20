#include "header.h"
#include <time.h>
#include <stdlib.h>

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

    // Chargement des feuilles
    SDL_Surface *feuille1 = IMG_Load("feuille1.png");
    SDL_Surface *feuille2 = IMG_Load("feuille2.png");

    if (!feuille1 || !feuille2) {
        printf("Erreur de chargement des images de feuilles\n");
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    Feuille feuillesBG1[NB_FEUILLes];
    Feuille feuillesBG2[NB_FEUILLes];
    srand(time(NULL));

    for (int i = 0; i < NB_FEUILLes; i++) {
        int x1 = rand() % (900 - 50);
        int y1 = rand() % (-200);
        int vitesse1 = 1 + rand() % 3;
        SDL_Surface *img1 = (rand() % 2 == 0) ? feuille1 : feuille2;
        initialiserFeuille(&feuillesBG1[i], img1, x1, y1, vitesse1);

        int x2 = 900 + rand() % (900 - 50);
        int y2 = rand() % (-200);
        int vitesse2 = 1 + rand() % 3;
        SDL_Surface *img2 = (rand() % 2 == 0) ? feuille1 : feuille2;
        initialiserFeuille(&feuillesBG2[i], img2, x2, y2, vitesse2);
    }

    initialiserBackground(&bg1, "stage finale.png", 0);
    bg1.camera.x = 0;
    bg1.camera.y = 150;
    bg1.camera.w = ecran->w / 2;
    bg1.camera.h = ecran->h;
    bg1.posEcran.x = 0;
    bg1.posEcran.y = 0;

    initialiserBackground(&bg2, "stage finale.png", ecran->w / 2);
    bg2.camera.w = ecran->w / 2;
    bg2.camera.h = ecran->h;
    bg2.posEcran.x = ecran->w / 2;
    bg2.posEcran.y = 0;

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

                    for (int i = 0; i < NB_FEUILLes; i++) {
                        int x = rand() % (1800 - 50);
                        int y = rand() % (-200);
                        int vitesse = 1 + rand() % 3;
                        SDL_Surface *img = (rand() % 2 == 0) ? feuille1 : feuille2;
                        initialiserFeuille(&feuillesBG1[i], img, x, y, vitesse);
                    }
                }

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
                    etatActuel = MODE_MULTI;
                    multiInit = 0;
                    break;
                }

                scrollingArrows(&bg1, keys);
                SDL_BlitSurface(bg1.img, &bg1.camera, ecran, &bg1.posEcran);

                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuillesBG1[i], 875, feuille1, feuille2);
                    afficherFeuille(feuillesBG1[i], ecran);
                }

                afficherTemps(ecran, debut, police);
                afficherGuide(ecran, "Appuyez sur 2 pour passer en mode multi", police);
                SDL_Flip(ecran);
                break;
            }

            case MODE_MULTI: {
                if (!multiInit) {
		   initialiserBackground(&bg1, "stage finale.png", 0);

		   bg1.camera.x = 0;
	           bg1.camera.y = 150;
		   bg1.camera.w = ecran->w / 2;
		   bg1.camera.h = ecran->h;
		   bg1.posEcran.x = 0;
		   bg1.posEcran.y = 0;

                    SDL_FreeSurface(bg2.img);
                    initialiserBackground(&bg2, "stage finale.png", ecran->w / 2);
                    bg2.camera.x = 0;
                    bg2.camera.y = 150;
                    bg2.camera.w = ecran->w / 2;
                    bg2.camera.h = ecran->h;
                    bg2.posEcran.x = ecran->w / 2;
                    bg2.posEcran.y = 0;

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

                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuillesBG1[i], 875, feuille1, feuille2);
                    if (feuillesBG1[i].pos.x < 900) {
                        afficherFeuille(feuillesBG1[i], ecran);
                    }
                }

                SDL_BlitSurface(bg2.img, &bg2.camera, ecran, &bg2.posEcran);

                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuillesBG2[i], 875, feuille1, feuille2);
                    if (feuillesBG2[i].pos.x >= 900) {
                        afficherFeuille(feuillesBG2[i], ecran);
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
    SDL_FreeSurface(bg1.img);
    SDL_FreeSurface(bg2.img);
    SDL_Quit();
    TTF_Quit();

    return 0;
}

