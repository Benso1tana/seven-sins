#include "header.h"
#include <time.h>

#define NB_FLAMMES_TOTALE 16
#define NB_CRISTAUX 20
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
    int score = 0;

    SDL_Surface *flammeBleueImg = IMG_Load("f1.png");
    SDL_Surface *flammeRougeImg = IMG_Load("f2.png");
    SDL_Surface *cristalImg = IMG_Load("cristale.png");

    if (!flammeBleueImg || !flammeRougeImg || !cristalImg) {
        printf("Erreur de chargement des images\n");
        SDL_Quit(); TTF_Quit(); return 1;
    }

    Flamme flammes[NB_FLAMMES_TOTALE];
    Cristal cristaux[NB_CRISTAUX];

    srand(time(NULL));
    for (int i = 0; i < NB_FLAMMES_TOTALE; i++) {
        int y = -(rand() % 400);
        int vitesse = 7 + rand() % 3;
        int x = (i + 1) * (LARGEUR_BG / (NB_FLAMMES_TOTALE + 1));
        SDL_Surface *img = (i % 2 == 0) ? flammeRougeImg : flammeBleueImg;
        initialiserFlamme(&flammes[i], img, x, y, vitesse);
    }

    for (int i = 0; i < NB_CRISTAUX; i++) {
        int x = rand() % (LARGEUR_BG - 50) + 50;
        int y = (rand() % 2 == 0) ? 640 : 400;
        cristaux[i].pos.x = x;
        cristaux[i].pos.y = y;
        cristaux[i].visible = 1;
    }

    initialiserBackground(&bg, "bg2.png", 0);
    initialiserKaelios(&kaelios, NULL);
    kaelios.pos.y = 610; // Corriger la position du perso

    int continuer = 1;
    while (continuer) {
        SDL_PollEvent(&event);
        const Uint8 *keys = SDL_GetKeyState(NULL);
        if (event.type == SDL_QUIT || (kaelios.vie <= 0 && keys[SDLK_ESCAPE]))
            continuer = 0;

        scrollingArrows(&bg, keys);

        if (kaelios.vie > 0) {
            deplacerKaelios(&kaelios, keys, bg);
            verifierCollisionFlammes(&kaelios, flammes, NB_FLAMMES_TOTALE);
        }

        // Collision avec cristaux
        for (int i = 0; i < NB_CRISTAUX; i++) {
            if (cristaux[i].visible &&
                kaelios.pos.x < cristaux[i].pos.x + cristalImg->w &&
                kaelios.pos.x + kaelios.pos.w > cristaux[i].pos.x &&
                kaelios.pos.y < cristaux[i].pos.y + cristalImg->h &&
                kaelios.pos.y + kaelios.pos.h > cristaux[i].pos.y) {

                cristaux[i].visible = 0;
                score++;
            }
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        afficherBackground(bg, ecran);

        Uint32 maintenant = SDL_GetTicks();
        for (int i = 0; i < NB_FLAMMES_TOTALE; i++) {
            animerFlamme(&flammes[i], HAUTEUR_ECRAN, flammeRougeImg, flammeBleueImg);
            afficherFlamme(flammes[i], ecran, bg);
        }

        for (int i = 0; i < NB_CRISTAUX; i++) {
            if (cristaux[i].visible) {
                SDL_Rect dst = {
                    cristaux[i].pos.x - bg.camera.x,
                    cristaux[i].pos.y - bg.camera.y,
                    cristalImg->w,
                    cristalImg->h
                };
                SDL_BlitSurface(cristalImg, NULL, ecran, &dst);
            }
        }

        afficherKaelios(&kaelios, ecran, bg);
        afficherBarreVie(&kaelios, ecran);
        afficherTemps(ecran, debut, police);

        char scoreTxt[50];
        sprintf(scoreTxt, "Cristaux : %d", score);
        SDL_Color jaune = {255, 255, 0};
        SDL_Surface *texte = TTF_RenderText_Solid(police, scoreTxt, jaune);
        SDL_Rect pos = {10, 40};
        SDL_BlitSurface(texte, NULL, ecran, &pos);
        SDL_FreeSurface(texte);

        if (kaelios.vie > 0)
            afficherGuide(ecran, "Evitez les flammes !", police);
        else
            afficherGuide(ecran, "GAME OVER - Appuyez sur Echap", police);

        SDL_Flip(ecran);
        SDL_Delay(16);
    }

    SDL_FreeSurface(flammeBleueImg);
    SDL_FreeSurface(flammeRougeImg);
    SDL_FreeSurface(cristalImg);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
