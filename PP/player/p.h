#ifndef P_H
#define P_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdbool.h> // For bool type
#include <SDL/SDL_ttf.h>
// Screen Dimensions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Player Constants
#define PLAYER_WALK_SPEED 4
#define PLAYER_RUN_SPEED 8
#define JUMP_POWER 15
#define GRAVITY 1
#define GROUND_Y (SCREEN_HEIGHT - 150) // Adjust based on your sprite height and desired ground level
#define JUMP_FRAMES 1

// Animation Frames (Adjust if you have different counts)
#define WALK_FRAMES 3
#define RUN_FRAMES 3
#define DOWN_FRAMES 2 // sol1, sol2
#define ATTACK_FRAMES 2

// Animation Delay (Milliseconds between frames)
#define ANIMATION_DELAY 100

// Directions
typedef enum {
    DIR_RIGHT = 0,
    DIR_LEFT = 1
} Direction;
typedef struct {
    int score;
    int temps;
    char playername[20];
} scoreinfo;
typedef struct {
    int nbvies; 
    SDL_Surface* coeur;
    SDL_Rect pos_coeur;
} NombreDeVie;
// Actions
typedef enum {
    ACTION_IDLE,
    ACTION_WALK,
    ACTION_RUN,
    ACTION_CROUCH,
    ACTION_JUMP,
    ACTION_DOWN, // Lying on ground
    ACTION_ATTACK
} Action;

// Player Structure
typedef struct {
    SDL_Rect pos; // Position (x, y) and Dimensions (w, h)
    int velX, velY; // Velocity

    Direction currentDir;
    Action currentAction;
    Action previousAction; // To return to previous state after attack/jump etc.

    int currentFrame;
    Uint32 lastFrameTime; // For animation timing

    bool isJumping;
    bool isOnGround;
    bool isAttacking; // Flag to manage attack animation duration

    // Surfaces for each state and direction
    SDL_Surface* idle[2]; // [DIR_RIGHT], [DIR_LEFT]
    SDL_Surface* walk[2][WALK_FRAMES]; // [Direction][FrameIndex]
    SDL_Surface* run[2][RUN_FRAMES];
    SDL_Surface* crouch[2];
    SDL_Surface* down[2][DOWN_FRAMES];
    SDL_Surface* attack[2][ATTACK_FRAMES];
    SDL_Surface* jumpSurface[2]; // Using the single 'jump.jpg' for both directions initially

} Player;

// --- Function Prototypes ---

// Helper to load images and set color key
SDL_Surface* loadImage(const char* filename);

// Player lifecycle
Player* createPlayer(int startX, int startY);
void destroyPlayer(Player* player);

// Player media loading
bool loadPlayerMedia(Player* player);

// Player logic
void handlePlayerInput(Player* player, const Uint8* keystates, scoreinfo* score);
bool loadPlayerMedia2(Player* player);
void updatePlayer(Player* player);
void handleInput(SDL_Event event, Player* player);
void handlePlayerInput2(Player* player, const Uint8* keystates, scoreinfo* score);
Player* createPlayer2(int startX, int startY);
void renderPlayer(Player* player, SDL_Surface* screen);
void demander_nom_joueur(char *playername);
void initialiser_score(scoreinfo* score);
void augmenter_score(scoreinfo *score);
void afficher_score(SDL_Surface* screen, TTF_Font* police, scoreinfo* score);
void enregistrer_scores(scoreinfo* score, char* filename);

void initialiser_coeur1(NombreDeVie *c);
void afficher_coeur1(SDL_Surface *screen, NombreDeVie c);
void liberer_coeur1(NombreDeVie *c);


void initialiser_coeur2(NombreDeVie *c);
void afficher_coeur2(SDL_Surface *screen, NombreDeVie c);
void liberer_coeur2(NombreDeVie *c);


void initialiser_coeur3(NombreDeVie *c);
void afficher_coeur3(SDL_Surface *screen, NombreDeVie c);
void liberer_coeur3(NombreDeVie *c);

#endif // PLAYER_H
