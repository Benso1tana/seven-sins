#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Screen Dimensions
#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 875

// Player Constants
#define PLAYER_WALK_SPEED 4
#define PLAYER_RUN_SPEED 8
#define JUMP_POWER 15
#define GRAVITY 1
#define GROUND_Y (SCREEN_HEIGHT - 150)
#define JUMP_FRAMES 1

// Animation Frames
#define WALK_FRAMES 3
#define RUN_FRAMES 3
#define DOWN_FRAMES 2
#define ATTACK_FRAMES 2

// Animation Delay
#define ANIMATION_DELAY 100

// Directions
typedef enum {
    DIR_RIGHT = 0,
    DIR_LEFT = 1
} Direction;

// Actions
typedef enum {
    ACTION_IDLE,
    ACTION_WALK,
    ACTION_RUN,
    ACTION_CROUCH,
    ACTION_JUMP,
    ACTION_DOWN,
    ACTION_ATTACK
} Action;

// Player Structure
typedef struct {
    SDL_Rect pos;
    int velX, velY;
    Direction currentDir;
    Action currentAction;
    Action previousAction;
    int currentFrame;
    Uint32 lastFrameTime;
    bool isJumping;
    bool isOnGround;
    bool isAttacking;
    SDL_Surface* idle[2];
    SDL_Surface* walk[2][WALK_FRAMES];
    SDL_Surface* run[2][RUN_FRAMES];
    SDL_Surface* crouch[2];
    SDL_Surface* down[2][DOWN_FRAMES];
    SDL_Surface* attack[2][ATTACK_FRAMES];
    SDL_Surface* jumpSurface[2];
} Player;

// Structure pour le background
typedef struct {
    SDL_Surface *img;
    SDL_Rect posEcran;
    SDL_Rect camera;
} Background;

// Player Functions
SDL_Surface* loadImage(const char* filename);
Player* createPlayer(int startX, int startY);
void destroyPlayer(Player* player);
bool loadPlayerMedia(Player* player);
void handlePlayerInput(Player* player, const Uint8* keystates);
void updatePlayer(Player* player);
void renderPlayer(Player* player, SDL_Surface* screen);

// Background Functions
void initialiserBackground(Background *bg, const char *path, int xPosEcran);
void afficherBackground(Background bg, SDL_Surface *ecran);

// Text Functions
void afficherTexte(SDL_Surface *ecran, const char *texte, TTF_Font *police, int x, int y);

#endif // HEADER_H 