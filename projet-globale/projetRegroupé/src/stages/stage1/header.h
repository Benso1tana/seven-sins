#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../../common.h"
#include <SDL/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Screen Dimensions
#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 875

// Animation Constants
#define NB_FEUILLes 100
#define NB_ETOILES 20
#define NB_COINS 14

// Structure pour les étoiles
typedef struct {
    SDL_Rect pos;
    float phase;
    float vitesse;
    int luminosite;
} Etoile;

// Structure pour les feuilles
typedef struct {
    SDL_Surface *img;
    SDL_Rect pos;
    int vitesse;
    int xOrigine;
} Feuille;

// Structure pour le background
typedef struct {
    SDL_Surface *img;
    SDL_Rect posEcran;
    SDL_Rect camera;
} Background;

// Structure pour les coins
typedef struct {
    SDL_Surface *img;
    SDL_Rect pos;
} Coin;

// États du jeu
typedef enum {
    MENU_PRINCIPAL,
    MODE_SOLO,
    MODE_MULTI,
    QUITTER
} EtatJeu;

void initialiserBackgroundStage1(Background *bg, const char *path, int xPosEcran);
void afficherBackgroundStage1(Background bg, SDL_Surface *ecran);
void scrollingKeysStage1(Background *bg, const Uint8 *keys);
void initialiserFeuilles(void);

#endif // HEADER_H

