#include "header.h"
#include <math.h>   // Pour sin()
#include <stdlib.h> // Pour rand()

// Dimensions de l'écran
#define LARGEUR_ECRAN 1800
#define HAUTEUR_ECRAN 875

void initialiserEtoiles(Etoile *etoiles, int nbEtoiles, SDL_Surface *backgroundImage) {
    for (int i = 0; i < nbEtoiles; i++) {
        etoiles[i].pos.x = rand() % backgroundImage->w;
        etoiles[i].pos.y = rand() % (backgroundImage->h / 2); // Ciels sombres
        etoiles[i].phase = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
        etoiles[i].vitesse = (float)(rand() % 3 + 1) / 100.0f;
        etoiles[i].luminosite = 0;
    }
}

void animerEtoiles(Etoile *etoiles, int nbEtoiles, float temps) {
    for (int i = 0; i < nbEtoiles; i++) {
        float alpha = (sin(temps * etoiles[i].vitesse + etoiles[i].phase) + 1.0f) * 127.5f;
        etoiles[i].luminosite = (int)alpha;
    }
}

void afficherEtoiles(Etoile *etoiles, int nbEtoiles, SDL_Surface *ecran, SDL_Surface *etoileSurface, SDL_Rect *camera) {
    for (int i = 0; i < nbEtoiles; i++) {
        // Position visuelle en fonction de la caméra
        SDL_Rect dst = etoiles[i].pos;
        dst.x -= camera->x;
        dst.y -= camera->y;

        Uint8 alpha = (Uint8)etoiles[i].luminosite;

        SDL_SetAlpha(etoileSurface, SDL_SRCALPHA, alpha);
        SDL_BlitSurface(etoileSurface, NULL, ecran, &dst);
    }
}

// Fonctions background et feuilles existantes...

void initialiserBackground(Background *bg, const char *path, int xPosEcran) {
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

void afficherBackground(Background bg, SDL_Surface *ecran) {
    SDL_BlitSurface(bg.img, &bg.camera, ecran, &bg.posEcran);
}

void scrollingArrows(Background *bg, const Uint8 *keys) {
    if (keys[SDLK_UP]) bg->camera.y -= 10;
    if (keys[SDLK_DOWN]) bg->camera.y += 10;
    if (keys[SDLK_LEFT]) bg->camera.x -= 10;
    if (keys[SDLK_RIGHT]) bg->camera.x += 10;

    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.x > bg->img->w - bg->camera.w) bg->camera.x = bg->img->w - bg->camera.w;
    if (bg->camera.y > bg->img->h - bg->camera.h) bg->camera.y = bg->img->h - bg->camera.h;
}

void scrollingKeys(Background *bg, const Uint8 *keys) {
    if (keys[SDLK_w]) bg->camera.y -= 10;
    if (keys[SDLK_s]) bg->camera.y += 10;
    if (keys[SDLK_a]) bg->camera.x -= 10;
    if (keys[SDLK_d]) bg->camera.x += 10;

    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.x > bg->img->w - bg->camera.w) bg->camera.x = bg->img->w - bg->camera.w;
    if (bg->camera.y > bg->img->h - bg->camera.h) bg->camera.y = bg->img->h - bg->camera.h;
}

void afficherTemps(SDL_Surface *ecran, Uint32 debut, TTF_Font *police) {
    SDL_Color couleur = {255, 255, 255};
    char temps[50];
    Uint32 maintenant = SDL_GetTicks();
    sprintf(temps, "Temps : %d s", (maintenant - debut) / 1000);

    SDL_Surface *texte = TTF_RenderText_Solid(police, temps, couleur);
    SDL_Rect pos = {10, 10};
    SDL_BlitSurface(texte, NULL, ecran, &pos);
    SDL_FreeSurface(texte);
}

void afficherGuide(SDL_Surface *ecran, const char *texte, TTF_Font *police) {
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *message = TTF_RenderText_Solid(police, texte, couleur);
    SDL_Rect pos = {500, 350};
    SDL_BlitSurface(message, NULL, ecran, &pos);
    SDL_FreeSurface(message);
}

void initialiserFeuille(Feuille *f, SDL_Surface *img, int x, int y, int vitesse) {
    f->img = img;
    f->pos.x = x;
    f->pos.y = y;
    f->vitesse = vitesse;
    f->xOrigine = x;
}

void animerFeuille(Feuille *f, int hauteurEcran, SDL_Surface *feuille1, SDL_Surface *feuille2) {
    f->pos.y += f->vitesse;

    if (f->pos.y > hauteurEcran) {
        f->pos.y = -f->img->h;
        f->xOrigine = rand() % (1800 - f->img->w);
        f->img = (rand() % 2 == 0) ? feuille1 : feuille2;
    }

    f->pos.x = f->xOrigine; // Fixe par rapport à la caméra
}

void afficherFeuille(Feuille f, SDL_Surface *ecran) {
    SDL_BlitSurface(f.img, NULL, ecran, &f.pos);
}
