#include "header.h"
#include <math.h>   // Pour sin()
#include <stdlib.h> // Pour rand()

// Dimensions de l'écran
#define LARGEUR_ECRAN 1800
#define HAUTEUR_ECRAN 875

void initialiserBackgroundStageFinale(Background *bg, const char *path, int xPosEcran) {
    bg->img = IMG_Load(path);
    if (!bg->img) {
        printf("Erreur de chargement image: %s\n", IMG_GetError());
        return;
    }

    bg->posEcran.x = xPosEcran;
    bg->posEcran.y = 0;

    bg->camera.x = 0;
    bg->camera.y = 0;
    bg->camera.w = 640;
    bg->camera.h = 800;
}

void afficherBackgroundStageFinale(Background bg, SDL_Surface *ecran) {
    SDL_BlitSurface(bg.img, &bg.camera, ecran, &bg.posEcran);
}

static void scrollingArrows(Background *bg, const Uint8 *keys) {
    if (keys[SDLK_UP]) bg->camera.y -= 10;
    if (keys[SDLK_DOWN]) bg->camera.y += 10;
    if (keys[SDLK_LEFT]) bg->camera.x -= 10;
    if (keys[SDLK_RIGHT]) bg->camera.x += 10;

    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.x > bg->img->w - bg->camera.w) bg->camera.x = bg->img->w - bg->camera.w;
    if (bg->camera.y > bg->img->h - bg->camera.h) bg->camera.y = bg->img->h - bg->camera.h;
}

void scrollingKeysStageFinale(Background *bg, const Uint8 *keys) {
    if (keys[SDLK_w]) bg->camera.y -= 10;
    if (keys[SDLK_s]) bg->camera.y += 10;
    if (keys[SDLK_a]) bg->camera.x -= 10;
    if (keys[SDLK_d]) bg->camera.x += 10;

    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.x > bg->img->w - bg->camera.w) bg->camera.x = bg->img->w - bg->camera.w;
    if (bg->camera.y > bg->img->h - bg->camera.h) bg->camera.y = bg->img->h - bg->camera.h;
}

static void afficherTemps(SDL_Surface *ecran, Uint32 debut, TTF_Font *police) {
    SDL_Color couleur = {255, 255, 255};
    char temps[50];
    Uint32 maintenant = SDL_GetTicks();
    sprintf(temps, "Temps : %d s", (maintenant - debut) / 1000);

    SDL_Surface *texte = TTF_RenderText_Solid(police, temps, couleur);
    SDL_Rect pos = {10, 10};
    SDL_BlitSurface(texte, NULL, ecran, &pos);
    SDL_FreeSurface(texte);
}

static void afficherGuide(SDL_Surface *ecran, const char *texte, TTF_Font *police) {
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *message = TTF_RenderText_Solid(police, texte, couleur);
    SDL_Rect pos = {500, 350};
    SDL_BlitSurface(message, NULL, ecran, &pos);
    SDL_FreeSurface(message);
}

static void initialiserFeuille(Feuille *f, SDL_Surface *img, int x, int y, int vitesse) {
    f->img = img;
    f->pos.x = x;
    f->pos.y = y;
    f->vitesse = vitesse;
    f->xOrigine = x;
}

static void animerFeuille(Feuille *f, int hauteurEcran, SDL_Surface *feuille1, SDL_Surface *feuille2) {
    f->pos.y += f->vitesse;

    if (f->pos.y > hauteurEcran) {
        f->pos.y = -f->img->h;
        f->xOrigine = rand() % (1800 - f->img->w);
        f->img = (rand() % 2 == 0) ? feuille1 : feuille2;
    }

    f->pos.x = f->xOrigine; // Fixe par rapport à la caméra
}

static void afficherFeuille(Feuille f, SDL_Surface *ecran) {
    SDL_BlitSurface(f.img, NULL, ecran, &f.pos);
}
