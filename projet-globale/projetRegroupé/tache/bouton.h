#ifndef BOUTON_H
#define BOUTON_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

// Structure de la mini-map
typedef struct {
    SDL_Surface *image;        // Image de la mini-map
    SDL_Rect position;         // Position sur l’écran
    float scale;               // Facteur d’échelle par rapport à la carte principale
    SDL_Surface *playerImage;  // Image représentant le joueur sur la mini-map
    SDL_Rect playerIcon;       // Position de l’icône du joueur dans la mini-map
    int level;                 // Niveau associé à la mini-map
} MiniMap;

// Fonctions de gestion de la mini-map
void initMiniMap(MiniMap *map, int level);
void updateMiniMap(MiniMap *map, SDL_Rect playerPosition);
void renderMiniMap(SDL_Surface *screen, MiniMap *map);
void drawPlayerOnMiniMap(SDL_Surface *screen, MiniMap *map, SDL_Rect playerPosition);
void MAJMinimap(SDL_Rect posJoueur, MiniMap *map, SDL_Rect camera, int redimensionnement);
void afficher(MiniMap m, SDL_Surface *screen);
void Liberer(MiniMap *map);

// Fonctions de détection de collision
int checkCollisionPerfectPixel(SDL_Surface *surface, SDL_Rect *player);
int checkCollisionBB(SDL_Rect *a, SDL_Rect *b);


#endif // BOUTON_H

