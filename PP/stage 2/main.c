#include "header.h"
#include <time.h>

#define NB_FLAMMES_TOTALE 16
#define LARGEUR_BG 13185
#define HAUTEUR_ECRAN 875

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_EnableKeyRepeat(10, 10);

    SDL_Surface *ecran = SDL_SetVideoMode(1800, HAUTEUR_ECRAN, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Flammes + Kaelios", NULL);
    TTF_Font *police = TTF_OpenFont("DejaVuSans.ttf", 24);

    SDL_Event event;
    Uint32 debut = SDL_GetTicks();
    Background bg;
    Kaelios kaelios;

    SDL_Surface *flammeBleueImg = IMG_Load("f1.png");
    SDL_Surface *flammeRougeImg = IMG_Load("f2.png");
    if (!flammeBleueImg || !flammeRougeImg) {
        printf("Erreur de chargement des images\n");
        SDL_Quit(); TTF_Quit(); return 1;
    }

    Flamme flammes[NB_FLAMMES_TOTALE];
    srand(time(NULL));
    for (int i = 0; i < NB_FLAMMES_TOTALE; i++) {
        int y = -(rand() % 400);
        int vitesse = 7 + rand() % 3;
        int x = (i + 1) * (LARGEUR_BG / (NB_FLAMMES_TOTALE + 1));
        SDL_Surface *img = (i % 2 == 0) ? flammeRougeImg : flammeBleueImg;
        initialiserFlamme(&flammes[i], img, x, y, vitesse);
    }

    initialiserBackground(&bg, "bg2.png", 0);
    initialiserKaelios(&kaelios, NULL);

    int continuer = 1;
    while (continuer) {
        SDL_PollEvent(&event);
        const Uint8 *keys = SDL_GetKeyState(NULL);
        if (event.type == SDL_QUIT)
            continuer = 0;

        scrollingArrows(&bg, keys);
        deplacerKaelios(&kaelios, keys, bg);
        verifierCollisionFlammes(&kaelios, flammes, NB_FLAMMES_TOTALE);

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        afficherBackground(bg, ecran);

        Uint32 maintenant = SDL_GetTicks();
        for (int i = 0; i < NB_FLAMMES_TOTALE; i++) {
            animerFlamme(&flammes[i], HAUTEUR_ECRAN, flammeRougeImg, flammeBleueImg);
            afficherFlamme(flammes[i], ecran, bg);
        }

        afficherKaelios(&kaelios, ecran, bg);
        afficherBarreVie(&kaelios, ecran);
        afficherTemps(ecran, debut, police);
        afficherGuide(ecran, "Evitez les flammes !", police);

        SDL_Flip(ecran);
        SDL_Delay(16);
    }

    SDL_FreeSurface(flammeBleueImg);
    SDL_FreeSurface(flammeRougeImg);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
