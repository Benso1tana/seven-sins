#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    bg->camera.w = 1800;
    bg->camera.h = 875;
}

void afficherBackground(Background bg, SDL_Surface *ecran) {
    SDL_BlitSurface(bg.img, &bg.camera, ecran, &bg.posEcran);
}

void scrollingArrows(Background *bg, const Uint8 *keys) {
    if (keys[SDLK_LEFT])  bg->camera.x -= 5;
    if (keys[SDLK_RIGHT]) bg->camera.x += 5;
    if (keys[SDLK_UP])    bg->camera.y -= 5;
    if (keys[SDLK_DOWN])  bg->camera.y += 5;

    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.x > bg->img->w - bg->camera.w)
        bg->camera.x = bg->img->w - bg->camera.w;
    if (bg->camera.y > bg->img->h - bg->camera.h)
        bg->camera.y = bg->img->h - bg->camera.h;
}

void initialiserFlamme(Flamme *f, SDL_Surface *img, int x, int y, int vitesse) {
    f->img = img;
    f->pos.x = x;
    f->pos.y = y;
    f->vitesse = vitesse;
    f->xOrigine = x;
}

void animerFlamme(Flamme *f, int hauteurEcran, SDL_Surface *flamme1, SDL_Surface *flamme2) {
    f->pos.y += f->vitesse;
    if (f->pos.y > hauteurEcran) {
        f->pos.y = -f->img->h;
        f->vitesse = 7 + rand() % 3;
        f->img = (rand() % 2 == 0) ? flamme1 : flamme2;
    }
    f->pos.x = f->xOrigine;
}

void afficherFlamme(Flamme f, SDL_Surface *ecran, Background bg) {
    SDL_Rect dst = { f.pos.x - bg.camera.x, f.pos.y - bg.camera.y, f.img->w, f.img->h };
    SDL_BlitSurface(f.img, NULL, ecran, &dst);
}

void initialiserKaelios(Kaelios *k, SDL_Surface *img) {
    k->pos.x = 500;
    k->pos.y = 750;
    k->pos.w = 100;
    k->pos.h = 150;
    k->direction = 1;
    k->frameIndex = 0;
    k->vie = 100;
    k->nbCristaux = 0;
    k->gameOver = 0;

    for (int i = 0; i < 6; i++) {
        char pathD[100], pathG[100];
        sprintf(pathD, "droite stage 2/marcher%d.png", i + 1);
        sprintf(pathG, "gauche stage 2/marcher%d.png", i + 1);

        k->spriteDroite[i] = IMG_Load(pathD);
        k->spriteGauche[i] = IMG_Load(pathG);
    }
}

void deplacerKaelios(Kaelios *k, const Uint8 *keys, Background bg) {
    if (k->vie <= 0) return;

    if (keys[SDLK_RIGHT]) {
        k->pos.x += 5;
        k->direction = 1;
        k->frameIndex = (k->frameIndex + 1) % 6;
    } else if (keys[SDLK_LEFT]) {
        k->pos.x -= 5;
        k->direction = -1;
        k->frameIndex = (k->frameIndex + 1) % 6;
    }
}

void afficherKaelios(Kaelios *k, SDL_Surface *ecran, Background bg) {
    if (k->vie <= 0) return;
    SDL_Surface *currentSprite = (k->direction == 1) ? k->spriteDroite[k->frameIndex] : k->spriteGauche[k->frameIndex];

    SDL_Rect dst = { k->pos.x - bg.camera.x, k->pos.y - bg.camera.y, currentSprite->w, currentSprite->h };
    SDL_BlitSurface(currentSprite, NULL, ecran, &dst);
}

void verifierCollisionFlammes(Kaelios *k, Flamme *flammes, int nb) {
    for (int i = 0; i < nb; i++) {
        SDL_Rect fRect = flammes[i].pos;
        fRect.w = flammes[i].img->w;
        fRect.h = flammes[i].img->h;

        if (k->pos.x < fRect.x + fRect.w &&
            k->pos.x + k->pos.w > fRect.x &&
            k->pos.y < fRect.y + fRect.h &&
            k->pos.y + k->pos.h > fRect.y) {

            k->vie -= 1;
            if (k->vie <= 0) {
                k->vie = 0;
                k->gameOver = 1;
            }
        }
    }
}

void verifierCollisionCristaux(Kaelios *k, Cristal *cristaux, int nb) {
    for (int i = 0; i < nb; i++) {
        if (cristaux[i].collecte) continue;

        SDL_Rect c = cristaux[i].pos;
        if (k->pos.x < c.x + c.w &&
            k->pos.x + k->pos.w > c.x &&
            k->pos.y < c.y + c.h &&
            k->pos.y + k->pos.h > c.y) {

            cristaux[i].collecte = 1;
            k->nbCristaux++;
        }
    }
}

void afficherCristaux(Cristal *cristaux, int nb, SDL_Surface *ecran, SDL_Surface *img, Background bg) {
    for (int i = 0; i < nb; i++) {
        if (cristaux[i].collecte) continue;

        SDL_Rect dst = {
            cristaux[i].pos.x - bg.camera.x,
            cristaux[i].pos.y - bg.camera.y,
            img->w,
            img->h
        };
        SDL_BlitSurface(img, NULL, ecran, &dst);
    }
}

void afficherBarreVie(Kaelios *k, SDL_Surface *ecran) {
    SDL_Rect fond = { 1500, 30, 200, 20 };
    SDL_Rect barre = { 1500, 30, 2 * k->vie, 20 };
    SDL_FillRect(ecran, &fond, SDL_MapRGB(ecran->format, 255, 0, 0));
    SDL_FillRect(ecran, &barre, SDL_MapRGB(ecran->format, 0, 255, 0));
}

void afficherGuide(SDL_Surface *ecran, const char *texte, TTF_Font *police) {
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *message = TTF_RenderText_Blended(police, texte, couleur);
    SDL_Rect pos = {500, 350};
    SDL_BlitSurface(message, NULL, ecran, &pos);
    SDL_FreeSurface(message);
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

void afficherScoreCristaux(SDL_Surface *ecran, int score, TTF_Font *police) {
    SDL_Color jaune = {255, 255, 0};
    char texte[50];
    sprintf(texte, "Cristaux : %d", score);
    SDL_Surface *msg = TTF_RenderText_Solid(police, texte, jaune);
    SDL_Rect pos = {10, 40};
    SDL_BlitSurface(msg, NULL, ecran, &pos);
    SDL_FreeSurface(msg);
}
