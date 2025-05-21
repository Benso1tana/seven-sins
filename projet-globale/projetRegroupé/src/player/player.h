#ifndef P_H
#define P_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include "../common.h"

// Screen dimensions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768
#define GROUND_Y (SCREEN_HEIGHT - 150)

// Animation frames
#define WALK_FRAMES 6
#define RUN_FRAMES 3
#define DOWN_FRAMES 2
#define JUMP_FRAMES 1
#define ATTACK_FRAMES 2
#define ANIMATION_DELAY 100

// Physics
#define GRAVITY 1

// Direction enum
typedef enum {
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Action enum
typedef enum {
    ACTION_IDLE,
    ACTION_WALK,
    ACTION_RUN,
    ACTION_JUMP,
    ACTION_CROUCH,
    ACTION_DOWN,
    ACTION_ATTACK
} Action;

// Score info struct
typedef struct {
    char playername[50];
    int score;
    int temps;
} scoreinfo;

// Life counter struct
typedef struct {
    SDL_Surface* coeur;
    SDL_Rect pos_coeur;
} NombreDeVie;

// Player struct
typedef struct {
    SDL_Rect pos;
    float velX;
    float velY;
    Direction currentDir;
    Action currentAction;
    Action previousAction;
    int currentFrame;
    Uint32 lastFrameTime;
    bool isJumping;
    bool isOnGround;
    bool isAttacking;

    // Animation surfaces
    SDL_Surface* idle[2];      // [DIR_LEFT, DIR_RIGHT]
    SDL_Surface* crouch[2];    // [DIR_LEFT, DIR_RIGHT]
    SDL_Surface* jumpSurface[2]; // [DIR_LEFT, DIR_RIGHT]
    SDL_Surface* walk[2][WALK_FRAMES];  // [DIR_LEFT/DIR_RIGHT][frame]
    SDL_Surface* run[2][RUN_FRAMES];    // [DIR_LEFT/DIR_RIGHT][frame]
    SDL_Surface* down[2][DOWN_FRAMES];  // [DIR_LEFT/DIR_RIGHT][frame]
    SDL_Surface* attack[2][ATTACK_FRAMES]; // [DIR_LEFT/DIR_RIGHT][frame]
} Player;

// Function declarations
Player* createPlayer(int startX, int startY);
Player* createPlayer2(int startX, int startY);
void destroyPlayer(Player* player);
bool loadPlayerMedia(Player* player);
bool loadPlayerMedia2(Player* player);
void handlePlayerInput(Player* player, const Uint8* keystates, scoreinfo* score);
void handlePlayerInput2(Player* player, const Uint8* keystates, scoreinfo* score);
void updatePlayer(Player* player);
void renderPlayer(Player* player, SDL_Surface* screen);

// Score functions
void initialiser_score(scoreinfo* score);
void augmenter_score(scoreinfo* score);
void afficher_score(SDL_Surface* screen, TTF_Font* police, scoreinfo* score);
void enregistrer_scores(scoreinfo* score, char* filename);

// Life counter functions
void initialiser_coeur1(NombreDeVie* c);
void afficher_coeur1(SDL_Surface* screen, NombreDeVie c);
void liberer_coeur1(NombreDeVie* c);
void initialiser_coeur2(NombreDeVie* c);
void afficher_coeur2(SDL_Surface* screen, NombreDeVie c);
void liberer_coeur2(NombreDeVie* c);
void initialiser_coeur3(NombreDeVie* c);
void afficher_coeur3(SDL_Surface* screen, NombreDeVie c);
void liberer_coeur3(NombreDeVie* c);

#endif
