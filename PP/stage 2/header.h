#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

typedef enum {
    MENU_PRINCIPAL,
    MODE_SOLO,
    MODE_MULTI,
    QUITTER
} EtatJeu;

typedef struct {
    SDL_Surface *img;
    SDL_Rect camera;
    SDL_Rect posEcran;
} Background;

typedef struct {
    SDL_Rect pos;
    int xOrigine;
    SDL_Surface *img;
    int vitesse;
} Flamme;

typedef struct {
    SDL_Rect pos;
} Cristal;

typedef struct {
    SDL_Rect pos;
    SDL_Surface *spriteDroite[6];
    SDL_Surface *spriteGauche[6];
    int frameIndex;
    int direction;
    int vie;
} Kaelios;

// Flammes
void initialiserFlamme(Flamme *f, SDL_Surface *img, int x, int y, int vitesse);
void animerFlamme(Flamme *f, int hauteurEcran, SDL_Surface *flamme1, SDL_Surface *flamme2);
void afficherFlamme(Flamme f, SDL_Surface *ecran, Background bg);

// Kaelios
void initialiserKaelios(Kaelios *k, SDL_Surface *img);
void deplacerKaelios(Kaelios *k, const Uint8 *keys, Background bg);
void afficherKaelios(Kaelios *k, SDL_Surface *ecran, Background bg);
void verifierCollisionFlammes(Kaelios *k, Flamme *flammes, int nb);
void afficherBarreVie(Kaelios *k, SDL_Surface *ecran);

// Background
void initialiserBackground(Background *bg, const char *path, int xPosEcran);
void afficherBackground(Background bg, SDL_Surface *ecran);
void scrollingArrows(Background *bg, const Uint8 *keys);

// Utilitaires
void afficherGuide(SDL_Surface *ecran, const char *texte, TTF_Font *police);
void afficherTemps(SDL_Surface *ecran, Uint32 debut, TTF_Font *police);

#endif // HEADER_H
