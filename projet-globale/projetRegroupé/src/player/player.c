#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

// Définir la taille écran (à ajuster si besoin)
#define SCREEN_H 1920
#define SCREEN_W 1080

// Attention : adapter ce chemin selon ton dossier réel
#define ROOT_PATH "/home/vboxuser/Desktop/khalil perso/"

// At the top, set the correct number of frames based on assets
#undef WALK_FRAMES
#define WALK_FRAMES 6
#undef RUN_FRAMES
#define RUN_FRAMES 3

// --- Helper Function ---
SDL_Surface* loadImage(const char* relativePath) {
    SDL_Surface* loadedImage = IMG_Load(relativePath);
    if (!loadedImage) {
        fprintf(stderr, "Failed to load image %s: %s\n", relativePath, IMG_GetError());
        return NULL;
    }

    SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage);

    if (!optimizedImage) {
        fprintf(stderr, "Failed to optimize image %s\n", relativePath);
        return NULL;
    }

    return optimizedImage;
}

// Helper to load and check a sprite
static SDL_Surface* loadAndCheck(const char* path, const char* action, int frame) {
    SDL_Surface* surf = loadImage(path);
    if (!surf) {
        if (frame >= 0)
            fprintf(stderr, "[SPRITE ERROR] Failed to load %s (frame %d): %s\n", path, frame, IMG_GetError());
        else
            fprintf(stderr, "[SPRITE ERROR] Failed to load %s: %s\n", path, IMG_GetError());
    }
    return surf;
}

// --- Player Lifecycle Functions ---
Player* createPlayer(int startX, int startY) {
    Player* player = (Player*)malloc(sizeof(Player));
    if (player == NULL) {
        fprintf(stderr, "Failed to allocate memory for Player\n");
        return NULL;
    }

    // Initialize state
    player->pos.x = startX;
    player->pos.y = startY;
    player->pos.w = 0; // Will be set after loading first image
    player->pos.h = 0;
    player->velX = 0;
    player->velY = 0;
    player->currentDir = DIR_RIGHT;
    player->currentAction = ACTION_IDLE;
    player->previousAction = ACTION_IDLE;
    player->currentFrame = 0;
    player->lastFrameTime = SDL_GetTicks();
    player->isJumping = false;
    player->isOnGround = true; // Assume starts on ground
    player->isAttacking = false;

    // Initialize all animation pointers to NULL
    for (int i = 0; i < 2; ++i) {
        player->idle[i] = NULL;
        player->crouch[i] = NULL;
        player->jumpSurface[i] = NULL;
        for (int j = 0; j < WALK_FRAMES; ++j) player->walk[i][j] = NULL;
        for (int j = 0; j < RUN_FRAMES; ++j) player->run[i][j] = NULL;
        for (int j = 0; j < DOWN_FRAMES; ++j) player->down[i][j] = NULL;
        for (int j = 0; j < ATTACK_FRAMES; ++j) player->attack[i][j] = NULL;
    }

    // Load media attempts to fill these surfaces
    if (!loadPlayerMedia(player)) {
        destroyPlayer(player); // Clean up if loading fails
        return NULL;
    }

    // Set initial dimensions based on the idle sprite
    if (player->idle[DIR_RIGHT]) {
        player->pos.w = player->idle[DIR_RIGHT]->w;
        player->pos.h = player->idle[DIR_RIGHT]->h;
        // Adjust start Y position to be on the ground correctly
        player->pos.y = GROUND_Y - player->pos.h;
    } else {
        fprintf(stderr, "Failed to load initial sprite to get dimensions.\n");
        destroyPlayer(player);
        return NULL;
    }

    return player;
}

Player* createPlayer2(int startX, int startY) {
    Player* player = (Player*)malloc(sizeof(Player));
    if (player == NULL) {
        fprintf(stderr, "Failed to allocate memory for Player\n");
        return NULL;
    }

    // Initialize state
    player->pos.x = startX;
    player->pos.y = startY;
    player->pos.w = 0; // Will be set after loading first image
    player->pos.h = 0;
    player->velX = 0;
    player->velY = 0;
    player->currentDir = DIR_RIGHT;
    player->currentAction = ACTION_IDLE;
    player->previousAction = ACTION_IDLE;
    player->currentFrame = 0;
    player->lastFrameTime = SDL_GetTicks();
    player->isJumping = false;
    player->isOnGround = true; // Assume starts on ground
    player->isAttacking = false;

    // Initialize all animation pointers to NULL
    for (int i = 0; i < 2; ++i) {
        player->idle[i] = NULL;
        player->crouch[i] = NULL;
        player->jumpSurface[i] = NULL;
        for (int j = 0; j < WALK_FRAMES; ++j) player->walk[i][j] = NULL;
        for (int j = 0; j < RUN_FRAMES; ++j) player->run[i][j] = NULL;
        for (int j = 0; j < DOWN_FRAMES; ++j) player->down[i][j] = NULL;
        for (int j = 0; j < ATTACK_FRAMES; ++j) player->attack[i][j] = NULL;
    }

    // Load media attempts to fill these surfaces
    if (!loadPlayerMedia2(player)) {
        destroyPlayer(player); // Clean up if loading fails
        return NULL;
    }

    // Set initial dimensions based on the idle sprite
    if (player->idle[DIR_RIGHT]) {
        player->pos.w = player->idle[DIR_RIGHT]->w;
        player->pos.h = player->idle[DIR_RIGHT]->h;
        // Adjust start Y position to be on the ground correctly
        player->pos.y = GROUND_Y - player->pos.h;
    } else {
        fprintf(stderr, "Failed to load initial sprite to get dimensions.\n");
        destroyPlayer(player);
        return NULL;
    }

    return player;
}

void destroyPlayer(Player* player) {
    if (player == NULL) return;

    // Free all surfaces
    for (int i = 0; i < 2; ++i) {
        SDL_FreeSurface(player->idle[i]);
        SDL_FreeSurface(player->crouch[i]);
        SDL_FreeSurface(player->jumpSurface[i]);
        for (int j = 0; j < WALK_FRAMES; ++j) SDL_FreeSurface(player->walk[i][j]);
        for (int j = 0; j < RUN_FRAMES; ++j) SDL_FreeSurface(player->run[i][j]);
        for (int j = 0; j < DOWN_FRAMES; ++j) SDL_FreeSurface(player->down[i][j]);
        for (int j = 0; j < ATTACK_FRAMES; ++j) SDL_FreeSurface(player->attack[i][j]);
    }

    free(player);
}

// --- Player Media Loading ---
bool loadPlayerMedia(Player* player) {
    // Right-facing images
    player->idle[DIR_RIGHT] = loadAndCheck("assets/images/sprites/droite/immobile.png", "idle right", -1);
    const char* walkR[6] = {"marcher1.png","marcher2.png","marcher3.png","marcher4.png","marcher5.png","marcher6.png"};
    char path[256];
    for (int i = 0; i < 6; ++i) {
        snprintf(path, sizeof(path), "assets/images/sprites/droite/%s", walkR[i]);
        player->walk[DIR_RIGHT][i] = loadAndCheck(path, "walk right", i);
    }
    const char* runR[3] = {"course1.png","course2.png","course3.png"};
    for (int i = 0; i < 3; ++i) {
        snprintf(path, sizeof(path), "assets/images/sprites/droite/%s", runR[i]);
        player->run[DIR_RIGHT][i] = loadAndCheck(path, "run right", i);
    }
    player->crouch[DIR_RIGHT] = loadAndCheck("assets/images/sprites/droite/accroupi.png", "crouch right", -1);
    player->down[DIR_RIGHT][0] = loadAndCheck("assets/images/sprites/droite/sol1.png", "down right", 0);
    player->down[DIR_RIGHT][1] = loadAndCheck("assets/images/sprites/droite/sol2.png", "down right", 1);
    player->attack[DIR_RIGHT][0] = loadAndCheck("assets/images/sprites/droite/attaque1.png", "attack right", 0);
    player->attack[DIR_RIGHT][1] = loadAndCheck("assets/images/sprites/droite/attaque2.png", "attack right", 1);
    player->jumpSurface[DIR_RIGHT] = loadAndCheck("assets/images/sprites/droite/jump.png", "jump right", -1);

    // Left-facing images
    player->idle[DIR_LEFT] = loadAndCheck("assets/images/sprites/gauche/immobile.png", "idle left", -1);
    const char* walkL[6] = {"marcher1.png","marcher2.png","marcher3.png","marcher4.png","marcher5.png","marcher6.png"};
    for (int i = 0; i < 6; ++i) {
        snprintf(path, sizeof(path), "assets/images/sprites/gauche/%s", walkL[i]);
        player->walk[DIR_LEFT][i] = loadAndCheck(path, "walk left", i);
    }
    const char* runL[3] = {"course1.png","course2.png","course3.png"};
    for (int i = 0; i < 3; ++i) {
        snprintf(path, sizeof(path), "assets/images/sprites/gauche/%s", runL[i]);
        player->run[DIR_LEFT][i] = loadAndCheck(path, "run left", i);
    }
    player->crouch[DIR_LEFT] = loadAndCheck("assets/images/sprites/gauche/accroupi.png", "crouch left", -1);
    player->down[DIR_LEFT][0] = loadAndCheck("assets/images/sprites/gauche/sol1.png", "down left", 0);
    player->down[DIR_LEFT][1] = loadAndCheck("assets/images/sprites/gauche/sol2.png", "down left", 1);
    player->attack[DIR_LEFT][0] = loadAndCheck("assets/images/sprites/gauche/attaque1.png", "attack left", 0);
    player->attack[DIR_LEFT][1] = loadAndCheck("assets/images/sprites/gauche/attaque2.png", "attack left", 1);
    player->jumpSurface[DIR_LEFT] = loadAndCheck("assets/images/sprites/gauche/jump.png", "jump left", -1);

    // Check for any NULL essential sprites
    if (!player->idle[DIR_RIGHT] || !player->idle[DIR_LEFT]) {
        fprintf(stderr, "Essential player images (idle) failed to load.\n");
        return false;
    }
    for (int i = 0; i < 6; ++i) {
        if (!player->walk[DIR_RIGHT][i] || !player->walk[DIR_LEFT][i]) return false;
    }
    for (int i = 0; i < 3; ++i) {
        if (!player->run[DIR_RIGHT][i] || !player->run[DIR_LEFT][i]) return false;
    }
    if (!player->crouch[DIR_RIGHT] || !player->crouch[DIR_LEFT]) return false;
    if (!player->down[DIR_RIGHT][0] || !player->down[DIR_RIGHT][1] || !player->down[DIR_LEFT][0] || !player->down[DIR_LEFT][1]) return false;
    if (!player->jumpSurface[DIR_RIGHT] || !player->jumpSurface[DIR_LEFT]) return false;
    // Attack is optional, but warn if missing
    if (!player->attack[DIR_RIGHT][0] || !player->attack[DIR_RIGHT][1]) fprintf(stderr, "[WARN] Missing attack right frames\n");
    if (!player->attack[DIR_LEFT][0] || !player->attack[DIR_LEFT][1]) fprintf(stderr, "[WARN] Missing attack left frames\n");
    return true;
}

bool loadPlayerMedia2(Player* player) {
    // Right-facing images
    player->idle[DIR_RIGHT] = loadImage("assets/images/sprites/droite/immobile.png");
    player->walk[DIR_RIGHT][0] = loadImage("assets/images/sprites/droite/marche1.png");
    player->walk[DIR_RIGHT][1] = loadImage("assets/images/sprites/droite/marche2.png");
    player->walk[DIR_RIGHT][2] = loadImage("assets/images/sprites/droite/marche3.png");
    player->run[DIR_RIGHT][0] = loadImage("assets/images/sprites/droite/course1.png");
    player->run[DIR_RIGHT][1] = loadImage("assets/images/sprites/droite/course2.png");
    player->run[DIR_RIGHT][2] = loadImage("assets/images/sprites/droite/course3.png");
    player->crouch[DIR_RIGHT] = loadImage("assets/images/sprites/droite/accroupi.png");
    player->down[DIR_RIGHT][0] = loadImage("assets/images/sprites/droite/sol1.png");
    player->down[DIR_RIGHT][1] = loadImage("assets/images/sprites/droite/sol2.png");
    player->attack[DIR_RIGHT][0] = loadImage("assets/images/sprites/droite/attaque1.png");
    player->attack[DIR_RIGHT][1] = loadImage("assets/images/sprites/droite/attaque2.png");
    player->jumpSurface[DIR_RIGHT] = loadImage("assets/images/sprites/droite/jump.png");

    // Left-facing images
    player->idle[DIR_LEFT] = loadImage("assets/images/sprites/gauche/immobile.png");
    player->walk[DIR_LEFT][0] = loadImage("assets/images/sprites/gauche/marche1.png");
    player->walk[DIR_LEFT][1] = loadImage("assets/images/sprites/gauche/marche2.png");
    player->walk[DIR_LEFT][2] = loadImage("assets/images/sprites/gauche/marche3.png");
    player->run[DIR_LEFT][0] = loadImage("assets/images/sprites/gauche/course1.png");
    player->run[DIR_LEFT][1] = loadImage("assets/images/sprites/gauche/course2.png");
    player->run[DIR_LEFT][2] = loadImage("assets/images/sprites/gauche/course3.png");
    player->crouch[DIR_LEFT] = loadImage("assets/images/sprites/gauche/accroupi.png");
    player->down[DIR_LEFT][0] = loadImage("assets/images/sprites/gauche/sol1.png");
    player->down[DIR_LEFT][1] = loadImage("assets/images/sprites/gauche/sol2.png");
    player->attack[DIR_LEFT][0] = loadImage("assets/images/sprites/gauche/attaque1.png");
    player->attack[DIR_LEFT][1] = loadImage("assets/images/sprites/gauche/attaque2.png");
    player->jumpSurface[DIR_LEFT] = loadImage("assets/images/sprites/gauche/jump.png");

    if (!player->idle[DIR_RIGHT] || !player->idle[DIR_LEFT]) {
        fprintf(stderr, "Essential player images (idle) failed to load.\n");
        return false;
    }

    return true;
}

// --- Player Logic Functions ---
void handlePlayerInput(Player* player, const Uint8* keystates, scoreinfo* score) {
    player->velX = 0;

    if (player->isAttacking) {
        return;
    }

    Action intendedAction = ACTION_IDLE;
    bool running = keystates[SDLK_LSHIFT] || keystates[SDLK_RSHIFT];

    if (keystates[SDLK_LCTRL] || keystates[SDLK_RCTRL]) {
        if (!player->isJumping && player->isOnGround) {
            intendedAction = ACTION_ATTACK;
            player->isAttacking = true;
            player->currentFrame = 0;
            player->lastFrameTime = SDL_GetTicks();
            augmenter_score(score);
        }

    } else if (keystates[SDLK_DOWN]) {
        if (!player->isJumping && player->isOnGround) {
            intendedAction = ACTION_CROUCH;
            augmenter_score(score);
        }

    } else if (keystates[SDLK_UP]) {
        if (!player->isJumping && player->isOnGround) {
            player->isJumping = true;
            player->velY = -15;
            intendedAction = ACTION_JUMP;
            augmenter_score(score);
        }

    } else if (keystates[SDLK_LEFT]) {
        player->velX = running ? -8 : -4;
        player->currentDir = DIR_LEFT;
        intendedAction = running ? ACTION_RUN : ACTION_WALK;
        augmenter_score(score);
    } else if (keystates[SDLK_RIGHT]) {
        player->velX = running ? 8 : 4;
        player->currentDir = DIR_RIGHT;
        intendedAction = running ? ACTION_RUN : ACTION_WALK;
        augmenter_score(score);
    }

    player->currentAction = intendedAction;
    printf("[INPUT] IntendedAction=%d, Dir=%d, Frame=%d\n", intendedAction, player->currentDir, player->currentFrame);
}

void handlePlayerInput2(Player* player, const Uint8* keystates, scoreinfo* score) {
    player->velX = 0;

    if (player->isAttacking) {
        return;
    }

    Action intendedAction = ACTION_IDLE;
    bool running = keystates[SDLK_p];  // Courir avec Ctrl gauche

    if (keystates[SDLK_f]) {  // Attaque avec F
        if (!player->isJumping && player->isOnGround) {
            intendedAction = ACTION_ATTACK;
            player->isAttacking = true;
            player->currentFrame = 0;
            player->lastFrameTime = SDL_GetTicks();
            augmenter_score(score);
        }

    } else if (keystates[SDLK_s]) {  // Accroupi avec S
        if (!player->isJumping && player->isOnGround) {
            intendedAction = ACTION_CROUCH;
            augmenter_score(score);
        }

    } else if (keystates[SDLK_w]) {  // Saut avec W
        if (!player->isJumping && player->isOnGround) {
            player->isJumping = true;
            player->velY = -15;
            intendedAction = ACTION_JUMP;
            augmenter_score(score);
        }

    } else if (keystates[SDLK_a]) {  // Gauche avec A
        player->velX = running ? -8 : -4;
        player->currentDir = DIR_LEFT;
        intendedAction = running ? ACTION_RUN : ACTION_WALK;
        augmenter_score(score);

    } else if (keystates[SDLK_d]) {  // Droite avec D
        player->velX = running ? 8 : 4;
        player->currentDir = DIR_RIGHT;
        intendedAction = running ? ACTION_RUN : ACTION_WALK;
        augmenter_score(score);
    }

    player->currentAction = intendedAction;
}

void updatePlayer(Player* player) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTime = currentTime - player->lastFrameTime;
    printf("[UPDATE] Action=%d, Dir=%d, Frame=%d\n", player->currentAction, player->currentDir, player->currentFrame);

    // Attack animation
    if (player->isAttacking) {
        if (deltaTime > ANIMATION_DELAY) {
            player->currentFrame++;
            player->lastFrameTime = currentTime;
            if (player->currentFrame >= ATTACK_FRAMES) {
                player->isAttacking = false;
                player->currentAction = ACTION_IDLE;
                player->currentFrame = 0;
            }
        }
        player->velX = 0;
    }
    // Other animations
    else if (deltaTime > ANIMATION_DELAY) {
        int maxFrames = 0;
        bool loop = true;

        switch (player->currentAction) {
            case ACTION_WALK:   maxFrames = WALK_FRAMES; break;
            case ACTION_RUN:    maxFrames = RUN_FRAMES; break;
            case ACTION_DOWN:   maxFrames = DOWN_FRAMES; break;
            case ACTION_JUMP:   maxFrames = JUMP_FRAMES; loop = false; break;
            case ACTION_IDLE:   maxFrames = 1; loop = false; break;
            case ACTION_CROUCH: maxFrames = 1; loop = false; break;
            default:            maxFrames = 0; break;
        }

        if (maxFrames > 0) {
            player->currentFrame++;
            if (player->currentFrame >= maxFrames) {
                if (loop) {
                    player->currentFrame = 0;
                } else {
                    player->currentFrame = maxFrames - 1;
                }
            }
            player->lastFrameTime = currentTime;
        } else {
            player->currentFrame = 0;
        }
    }

    // Physics
    if (!player->isOnGround) {
        player->velY += GRAVITY;
    }

    player->pos.x += player->velX;
    player->pos.y += player->velY;

    // Horizontal bounds
    if (player->pos.x < 0) {
        player->pos.x = 0;
    } else if (player->pos.x + player->pos.w > SCREEN_WIDTH) {
        player->pos.x = SCREEN_WIDTH - player->pos.w;
    }

    // Ground collision
    if (player->pos.y + player->pos.h >= GROUND_Y) {
        player->pos.y = GROUND_Y - player->pos.h;
        player->velY = 0;
        if (player->isJumping) {
            player->isJumping = false;
            Uint8* keys = SDL_GetKeyState(NULL);
            if (!keys[SDLK_LEFT] && !keys[SDLK_RIGHT] && !player->isAttacking) {
                player->currentAction = ACTION_IDLE;
            }
        }
        player->isOnGround = true;
    } else {
        player->isOnGround = false;
    }

    // Ceiling collision
    if (player->pos.y < 0) {
        player->pos.y = 0;
        player->velY = 0;
    }
}

void renderPlayer(Player* player, SDL_Surface* screen) {
    printf("[RENDER] Action=%d, Dir=%d, Frame=%d\n", player->currentAction, player->currentDir, player->currentFrame);
    SDL_Surface* frameToDraw = NULL;
    int frame = player->currentFrame;
    int dir = player->currentDir;

    switch (player->currentAction) {
        case ACTION_IDLE:
            frameToDraw = player->idle[dir];
            printf("[DEBUG] Rendering IDLE, dir=%d, surface=%p\n", dir, (void*)frameToDraw);
            break;
        case ACTION_WALK:
            if (frame < 0 || frame >= WALK_FRAMES) {
                printf("[ERROR] WALK frame index out of bounds: %d (max %d)\n", frame, WALK_FRAMES-1);
                frame = 0;
            }
            frameToDraw = player->walk[dir][frame];
            printf("[DEBUG] Rendering WALK, dir=%d, frame=%d/%d, surface=%p\n", dir, frame, WALK_FRAMES, (void*)frameToDraw);
            break;
        case ACTION_RUN:
            if (frame < 0 || frame >= RUN_FRAMES) {
                printf("[ERROR] RUN frame index out of bounds: %d (max %d)\n", frame, RUN_FRAMES-1);
                frame = 0;
            }
            frameToDraw = player->run[dir][frame];
            printf("[DEBUG] Rendering RUN, dir=%d, frame=%d/%d, surface=%p\n", dir, frame, RUN_FRAMES, (void*)frameToDraw);
            break;
        case ACTION_CROUCH:
            frameToDraw = player->crouch[dir];
            printf("[DEBUG] Rendering CROUCH, dir=%d, surface=%p\n", dir, (void*)frameToDraw);
            break;
        case ACTION_JUMP:
            frameToDraw = player->jumpSurface[dir];
            printf("[DEBUG] Rendering JUMP, dir=%d, surface=%p\n", dir, (void*)frameToDraw);
            break;
        case ACTION_DOWN:
            if (frame < 0 || frame >= 2) {
                printf("[ERROR] DOWN frame index out of bounds: %d (max 1)\n", frame);
                frame = 0;
            }
            frameToDraw = player->down[dir][frame];
            printf("[DEBUG] Rendering DOWN, dir=%d, frame=%d/2, surface=%p\n", dir, frame, (void*)frameToDraw);
            break;
        case ACTION_ATTACK:
            if (frame < 0 || frame >= 2) {
                printf("[ERROR] ATTACK frame index out of bounds: %d (max 1)\n", frame);
                frame = 0;
            }
            frameToDraw = player->attack[dir][frame];
            printf("[DEBUG] Rendering ATTACK, dir=%d, frame=%d/2, surface=%p\n", dir, frame, (void*)frameToDraw);
            break;
        default:
            frameToDraw = player->idle[dir];
            printf("[DEBUG] Rendering DEFAULT (IDLE), dir=%d, surface=%p\n", dir, (void*)frameToDraw);
            break;
    }

    if (frameToDraw != NULL) {
        SDL_Rect offset = player->pos;
        SDL_BlitSurface(frameToDraw, NULL, screen, &offset);
    } else {
        printf("[ERROR] Player frameToDraw is NULL! Action=%d, Dir=%d, Frame=%d\n", player->currentAction, dir, frame);
    }
}

// Les autres fonctions (gestion score, vie, coeur, etc.) restent inchangées
// Tu peux les rajouter ici sans modification.


void demander_nom_joueur(char *playername)
{
    printf("Entrez votre nom: ");
    scanf("%s", playername);
}

void initialiser_score(scoreinfo* score)
{
    score->score = 0;
    score->temps = 0;
    
    demander_nom_joueur(score->playername); // Remplit directement le champ playername
}

void augmenter_score(scoreinfo *score)
{
    score->score += 1;
}
void afficher_score(SDL_Surface* screen, TTF_Font* police, scoreinfo* score)
{
    char score_str[50];
    SDL_Color couleur = {0, 0, 0};  
    SDL_Rect pos_score;
   

    sprintf(score_str, "Score:%d", score->score);//bech tiktib min fichier
    
    SDL_Surface* surface_score = TTF_RenderText_Solid(police, score_str, couleur);
    
   
    pos_score.x = screen->w - surface_score->w - 10;
    pos_score.y = 10;

    SDL_BlitSurface(surface_score, NULL, screen, &pos_score);

    SDL_FreeSurface(surface_score);
}


void enregistrer_scores(scoreinfo* score, char* filename)
{
    FILE* fichier = fopen(filename, "a+");

    if (fichier != NULL) 
    {
        fprintf(fichier, "%s %d %d\n", score->playername, score->score, score->temps);
        fclose(fichier);
    }
}

void initialiser_coeur1(NombreDeVie *c)
{
	c->coeur=IMG_Load("coeur.png");
	if(c->coeur==NULL)
	{
		printf("Unable to load background image %s \n",SDL_GetError());
		return;
	}
	c->pos_coeur.x = 15;
	c->pos_coeur.y = 10;
	c->pos_coeur.h = SCREEN_H;
	c->pos_coeur.w = SCREEN_W;
}
void afficher_coeur1(SDL_Surface *screen,NombreDeVie c)
{
	SDL_BlitSurface(c.coeur,NULL,screen,&c.pos_coeur);
}

void liberer_coeur1(NombreDeVie *c)
{
	SDL_FreeSurface(c->coeur);
}

void initialiser_coeur2(NombreDeVie *c)
{
	c->coeur=IMG_Load("coeur.png");
	if(c->coeur==NULL)
	{
		printf("Unable to load background image %s \n",SDL_GetError());
		return;
	}
	c->pos_coeur.x = 55;
	c->pos_coeur.y = 10;
	c->pos_coeur.h = SCREEN_H;
	c->pos_coeur.w = SCREEN_W;
}
void afficher_coeur2(SDL_Surface *screen,NombreDeVie c)
{
	SDL_BlitSurface(c.coeur,NULL,screen,&c.pos_coeur);
}
void liberer_coeur2(NombreDeVie *c)
{
	SDL_FreeSurface(c->coeur);
}
void initialiser_coeur3(NombreDeVie *c)
{
	c->coeur=IMG_Load("coeur.png");
	if(c->coeur==NULL)
	{
		printf("Unable to load background image %s \n",SDL_GetError());
		return;
	}
	c->pos_coeur.x = 95;
	c->pos_coeur.y = 10;
	c->pos_coeur.h = SCREEN_H;
	c->pos_coeur.w = SCREEN_W;
}
void afficher_coeur3(SDL_Surface *screen,NombreDeVie c)
{
	SDL_BlitSurface(c.coeur,NULL,screen,&c.pos_coeur);
}
void liberer_coeur3(NombreDeVie *c)
{
	SDL_FreeSurface(c->coeur);
}


