#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "p.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int argc, char* argv[]) {
    SDL_Surface* screen = NULL;
    SDL_Surface* background = NULL;
    SDL_Event event;
    NombreDeVie c1, c2, c3;
    scoreinfo score;
    scoreinfo tab[2000];
    bool running = true;

    int scoreIndep = 0; // ✅ Nouveau score indépendant

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("police.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        return -1;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Screen could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption("SDL 1.2 Player Animation", NULL);

    background = loadImage("images/background.png");
    if (!background) {
        fprintf(stderr, "Failed to load background!\n");
        return 1;
    }

    SDL_Surface* temp = SDL_DisplayFormat(background);
    if (temp != NULL) {
        SDL_FreeSurface(background);
        background = temp;
    }

    initialiser_score(&score);
    initialiser_coeur1(&c1);
    initialiser_coeur2(&c2);
    initialiser_coeur3(&c3);

    Player* player1 = createPlayer(100, GROUND_Y);
    Player* player2 = createPlayer2(300, GROUND_Y);

    if (!player1 || !player2) {
        fprintf(stderr, "Erreur de création des joueurs.\n");
        return 1;
    }

    player1->pos.y = GROUND_Y - player1->pos.h;
    player2->pos.y = GROUND_Y - player2->pos.h;

    Uint32 lastTick = SDL_GetTicks();

    while (running) {
        Uint32 currentTick = SDL_GetTicks();
        lastTick = currentTick;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                enregistrer_scores(&score, "scores.txt");
            }
        }

        Uint8* keystates = SDL_GetKeyState(NULL);

        // ✅ Contrôle joueur 1
        int scoreBefore = scoreIndep;
        handlePlayerInput(player1, keystates, &score);
        if (keystates[SDLK_RIGHT] || keystates[SDLK_LEFT] || keystates[SDLK_UP] || keystates[SDLK_DOWN]) {
            scoreIndep += 10;
        }

        // ✅ Contrôle joueur 2
        handlePlayerInput2(player2, keystates, &score);
        if (keystates[SDLK_d] || keystates[SDLK_q] || keystates[SDLK_z] || keystates[SDLK_s]) {
            scoreIndep += 10;
        }

        updatePlayer(player1);
        updatePlayer(player2);

        SDL_BlitSurface(background, NULL, screen, NULL);

        afficher_score(screen, font, &score);
        afficher_coeur1(screen, c1);
        afficher_coeur2(screen, c2);
        afficher_coeur3(screen, c3);

        renderPlayer(player1, screen);
        renderPlayer(player2, screen);

        // ✅ Affichage du score indépendant
        char scoreText[64];
        sprintf(scoreText, "Score: %d", scoreIndep);
        SDL_Color color = {255, 255, 255};
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, color);
        SDL_Rect scorePos = {SCREEN_WIDTH - scoreSurface->w - 20, 10};
        SDL_BlitSurface(scoreSurface, NULL, screen, &scorePos);
        SDL_FreeSurface(scoreSurface);

        if (SDL_Flip(screen) == -1) {
            fprintf(stderr, "SDL_Flip failed: %s\n", SDL_GetError());
            running = false;
        }

        Uint32 frameTime = SDL_GetTicks() - currentTick;
        if (frameTime < 16) SDL_Delay(16 - frameTime);
    }

    destroyPlayer(player1);
    destroyPlayer(player2);
    SDL_FreeSurface(background);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

