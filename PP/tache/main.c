#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "bouton.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE);
    SDL_Rect camera = {0, 0, 800, 600};

    // Chargement du background par défaut
    SDL_Surface *background = IMG_Load("background.png");
    SDL_Surface *background_collision = IMG_Load("background_collision.png");

    if (!background || !background_collision) {
        fprintf(stderr, "Erreur de chargement des backgrounds: %s\n", SDL_GetError());
        return 1;
    }

    MiniMap map;
    SDL_Rect playerPosition = {100, 100, 32, 32};
    SDL_Rect platform = {300, 300, 100, 20};

    initMiniMap(&map, 1);

    SDL_Event event;
    int running = 1;
    int collisionDetected = 0; // False au départ

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  playerPosition.x -= 8; break;
                    case SDLK_RIGHT: playerPosition.x += 8; break;
                    case SDLK_UP:    playerPosition.y -= 8; break;
                    case SDLK_DOWN:  playerPosition.y += 8; break;
                    case SDLK_ESCAPE: running = 0; break;
                }
            }
        }

        // Contrôle des limites
        if (playerPosition.x < 0) playerPosition.x = 0;
        if (playerPosition.x > 1920 - playerPosition.w) playerPosition.x = 1920 - playerPosition.w;
        if (playerPosition.y < 0) playerPosition.y = 0;
        if (playerPosition.y > 1080 - playerPosition.h) playerPosition.y = 1080 - playerPosition.h;

        //updateMiniMap(&map, playerPosition);

        // Détection collision
        if (checkCollisionPerfectPixel(map.image, &playerPosition)) {
            if (!collisionDetected) {
                printf("Collision spéciale détectée !\n");
                collisionDetected = 1;
            }
        }

        if (checkCollisionBB(&playerPosition, &platform)) {
            printf("Collision plateforme !\n");
        }

        // Choix du fond en fonction de la collision
        SDL_Surface *currentBackground = collisionDetected ? background_collision : background;

        // Affichage
        SDL_BlitSurface(currentBackground, NULL, screen, NULL);

       // afficher(map, screen);
        MAJMinimap(playerPosition, &map, camera, 10);

        renderMiniMap(screen, &map);
        drawPlayerOnMiniMap(screen, &map, playerPosition);

        SDL_Flip(screen);
        SDL_Delay(16);
    }

    SDL_FreeSurface(background);
    SDL_FreeSurface(background_collision);
    Liberer(&map);

    SDL_Quit();
    return 0;
}

