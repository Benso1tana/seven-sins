#include "p.h"
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

// --- Helper Function ---
SDL_Surface* loadImage(const char* relativePath) {
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", ROOT_PATH, relativePath);

    SDL_Surface* loadedImage = IMG_Load(fullPath);
    if (!loadedImage) {
        fprintf(stderr, "Failed to load image %s: %s\n", fullPath, IMG_GetError());
        return NULL;
    }

    SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage);

    if (!optimizedImage) {
        fprintf(stderr, "Failed to optimize image %s\n", fullPath);
        return NULL;
    }

    return optimizedImage;
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

    // Initialize surface pointers to NULL
    for (int i = 0; i < 2; ++i) {
        player->idle[i] = NULL;
        player->crouch[i] = NULL;
        player->jumpSurface[i] = NULL; // Only one jump image provided initially
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

    // Initialize surface pointers to NULL
    for (int i = 0; i < 2; ++i) {
        player->idle[i] = NULL;
        player->crouch[i] = NULL;
        player->jumpSurface[i] = NULL; // Only one jump image provided initially
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
    player->idle[DIR_RIGHT] = loadImage("images/droite/immobile.png");
    player->walk[DIR_RIGHT][0] = loadImage("images/droite/marcher1.png");
    player->walk[DIR_RIGHT][1] = loadImage("images/droite/marcher6.png");
    player->walk[DIR_RIGHT][2] = loadImage("images/droite/marcher4.png");
    player->walk[DIR_RIGHT][3] = loadImage("images/droite/marcher5.png");
    player->walk[DIR_LEFT][4] = loadImage("images/gauche/marcher6.png");
    player->walk[DIR_RIGHT][5] = loadImage("images/droite/marcher1.png");
   
    player->run[DIR_RIGHT][0] = loadImage("images/droite/course1.png");
    player->run[DIR_RIGHT][1] = loadImage("images/droite/course2.png");
    player->run[DIR_RIGHT][2] = loadImage("images/droite/course3.png");
    player->crouch[DIR_RIGHT] = loadImage("images/droite/accroupi.png");
    player->down[DIR_RIGHT][0] = loadImage("images/droite/sol1.png");
    player->down[DIR_RIGHT][1] = loadImage("images/droite/sol2.png");
    player->attack[DIR_RIGHT][0] = loadImage("images/droite/attaque1.png");
    player->attack[DIR_RIGHT][1] = loadImage("images/droite/attaque2.png");

    // Left-facing images
    player->idle[DIR_LEFT] = loadImage("images/gauche/immobile.png");
    player->walk[DIR_LEFT][0] = loadImage("images/gauche/marcher1.png");
    player->walk[DIR_LEFT][1] = loadImage("images/gauche/marcher6.png");
    player->walk[DIR_LEFT][2] = loadImage("images/gauche/marcher4.png");
    player->walk[DIR_LEFT][3] = loadImage("images/gauche/marcher5.png");
    player->walk[DIR_LEFT][4] = loadImage("images/gauche/marcher6.png");
    player->walk[DIR_LEFT][5] = loadImage("images/gauche/marcher1.png");
    player->run[DIR_LEFT][0] = loadImage("images/gauche/course1.png");
    player->run[DIR_LEFT][1] = loadImage("images/gauche/course2.png");
    player->run[DIR_LEFT][2] = loadImage("images/gauche/course3.png");
    player->crouch[DIR_LEFT] = loadImage("images/gauche/accroupi.png");
    player->down[DIR_LEFT][0] = loadImage("images/gauche/sol1.png");
    player->down[DIR_LEFT][1] = loadImage("images/gauche/sol2.png");
    player->attack[DIR_LEFT][0] = loadImage("images/gauche/attaque1.png");
    player->attack[DIR_LEFT][1] = loadImage("images/gauche/attaque2.png");

    player->jumpSurface[DIR_RIGHT] = loadImage("images/droite/jump.png");
    player->jumpSurface[DIR_LEFT] = loadImage("images/gauche/jump.png");

    if (!player->idle[DIR_RIGHT] || !player->idle[DIR_LEFT]) {
        fprintf(stderr, "Essential player images (idle) failed to load.\n");
        return false;
    }

    return true;
}

bool loadPlayerMedia2(Player* player) {
    // Right-facing images
    player->idle[DIR_RIGHT] = loadImage("images2/droite/immobile.png");
    player->walk[DIR_RIGHT][0] = loadImage("images2/droite/marche1.png");
    player->walk[DIR_RIGHT][1] = loadImage("images2/droite/marche2.png");
    player->walk[DIR_RIGHT][2] = loadImage("images2/droite/marche3.png");
    player->run[DIR_RIGHT][0] = loadImage("images2/droite/course1.png");
    player->run[DIR_RIGHT][1] = loadImage("images2/droite/course2.png");
    player->run[DIR_RIGHT][2] = loadImage("images2/droite/course3.png");
    player->crouch[DIR_RIGHT] = loadImage("images2/droite/accroupi.png");
    player->down[DIR_RIGHT][0] = loadImage("images2/droite/sol1.png");
    player->down[DIR_RIGHT][1] = loadImage("images2/droite/sol2.png");
    player->attack[DIR_RIGHT][0] = loadImage("images2/droite/attaque1.png");
    player->attack[DIR_RIGHT][1] = loadImage("images2/droite/attaque2.png");

    // Left-facing images
    player->idle[DIR_LEFT] = loadImage("images2/gauche/immobile.png");
    player->walk[DIR_LEFT][0] = loadImage("images2/gauche/marche1.png");
    player->walk[DIR_LEFT][1] = loadImage("images2/gauche/marche2.png");
    player->walk[DIR_LEFT][2] = loadImage("images2/gauche/marche3.png");
    player->run[DIR_LEFT][0] = loadImage("images2/gauche/course1.png");
    player->run[DIR_LEFT][1] = loadImage("images2/gauche/course2.png");
    player->run[DIR_LEFT][2] = loadImage("images2/gauche/course3.png");
    player->crouch[DIR_LEFT] = loadImage("images2/gauche/accroupi.png");
    player->down[DIR_LEFT][0] = loadImage("images2/gauche/sol1.png");
    player->down[DIR_LEFT][1] = loadImage("images2/gauche/sol2.png");
    player->attack[DIR_LEFT][0] = loadImage("images2/gauche/attaque1.png");
    player->attack[DIR_LEFT][1] = loadImage("images2/gauche/attaque2.png");

    player->jumpSurface[DIR_RIGHT] = loadImage("images2/droite/jump.png");
    player->jumpSurface[DIR_LEFT] = loadImage("images2/gauche/jump.png");

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
    SDL_Surface* frameToDraw = NULL;

    switch (player->currentAction) {
        case ACTION_IDLE:
            frameToDraw = player->idle[player->currentDir];
            break;
        case ACTION_WALK:
            frameToDraw = player->walk[player->currentDir][player->currentFrame];
            break;
        case ACTION_RUN:
            frameToDraw = player->run[player->currentDir][player->currentFrame];
            break;
        case ACTION_CROUCH:
            frameToDraw = player->crouch[player->currentDir];
            break;
        case ACTION_JUMP:
            frameToDraw = player->jumpSurface[player->currentDir];
            break;
        case ACTION_DOWN:
            frameToDraw = player->down[player->currentDir][player->currentFrame];
            break;
        case ACTION_ATTACK:
            frameToDraw = player->attack[player->currentDir][player->currentFrame];
            break;
        default:
            frameToDraw = player->idle[player->currentDir];
            break;
    }

    if (frameToDraw != NULL) {
        SDL_Rect offset = player->pos;
        SDL_BlitSurface(frameToDraw, NULL, screen, &offset);
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


