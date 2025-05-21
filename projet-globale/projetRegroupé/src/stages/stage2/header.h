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

// Function declarations
void initialiserBackgroundStage2(Background *bg, const char *path, int xPosEcran);
void afficherBackgroundStage2(Background bg, SDL_Surface *ecran);

#endif // HEADER_H
