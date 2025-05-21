#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>

// Constantes
#define NB_FEUILLes 100
#define NB_ETOILES 20

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

// États du jeu
typedef enum {
    MENU_PRINCIPAL,
    MODE_SOLO,
    MODE_MULTI,
    QUITTER
} EtatJeu;

// Function declarations
void initialiserBackgroundStageFinale(Background *bg, const char *path, int xPosEcran);
void afficherBackgroundStageFinale(Background bg, SDL_Surface *ecran);
void scrollingKeysStageFinale(Background *bg, const Uint8 *keys);

#endif // HEADER_H
