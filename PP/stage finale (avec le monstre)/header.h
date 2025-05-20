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

// Fonctions background
void initialiserBackground(Background *bg, const char *path, int xPosEcran);
void afficherBackground(Background bg, SDL_Surface *ecran);
void scrollingArrows(Background *bg, const Uint8 *keys);
void scrollingKeys(Background *bg, const Uint8 *keys);

// Fonctions feuilles
void initialiserFeuille(Feuille *f, SDL_Surface *img, int x, int y, int vitesse);
void animerFeuille(Feuille *f, int hauteurEcran, SDL_Surface *feuille1, SDL_Surface *feuille2);
void afficherFeuille(Feuille f, SDL_Surface *ecran);



// Affichage texte
void afficherTemps(SDL_Surface *ecran, Uint32 debut, TTF_Font *police);
void afficherGuide(SDL_Surface *ecran, const char *texte, TTF_Font *police);

#endif // HEADER_H
