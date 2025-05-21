#include "header.h"

// Helper Function
SDL_Surface* loadImage(const char* filename) {
    SDL_Surface* loadedImage = IMG_Load(filename);
    if (!loadedImage) {
        printf("Failed to load image %s: %s\n", filename, IMG_GetError());
        return NULL;
    }

    SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage);

    if (!optimizedImage) {
        printf("Failed to optimize image %s\n", filename);
        return NULL;
    }

    return optimizedImage;
}

// Player Functions
Player* createPlayer(int startX, int startY) {
    Player* player = (Player*)malloc(sizeof(Player));
    if (!player) {
        printf("Failed to allocate memory for Player\n");
        return NULL;
    }

    // Initialize state
    player->pos.x = startX;
    player->pos.y = startY;
    player->pos.w = 0;
    player->pos.h = 0;
    player->velX = 0;
    player->velY = 0;
    player->currentDir = DIR_RIGHT;
    player->currentAction = ACTION_IDLE;
    player->previousAction = ACTION_IDLE;
    player->currentFrame = 0;
    player->lastFrameTime = SDL_GetTicks();
    player->isJumping = false;
    player->isOnGround = true;
    player->isAttacking = false;

    // Initialize surface pointers to NULL
    for (int i = 0; i < 2; ++i) {
        player->idle[i] = NULL;
        player->crouch[i] = NULL;
        player->jumpSurface[i] = NULL;
        for (int j = 0; j < WALK_FRAMES; ++j) player->walk[i][j] = NULL;
        for (int j = 0; j < RUN_FRAMES; ++j) player->run[i][j] = NULL;
        for (int j = 0; j < DOWN_FRAMES; ++j) player->down[i][j] = NULL;
        for (int j = 0; j < ATTACK_FRAMES; ++j) player->attack[i][j] = NULL;
    }

    if (!loadPlayerMedia(player)) {
        destroyPlayer(player);
        return NULL;
    }

    if (player->idle[DIR_RIGHT]) {
        player->pos.w = player->idle[DIR_RIGHT]->w;
        player->pos.h = player->idle[DIR_RIGHT]->h;
        player->pos.y = GROUND_Y - player->pos.h;
    } else {
        printf("Failed to load initial sprite\n");
        destroyPlayer(player);
        return NULL;
    }

    return player;
}

void destroyPlayer(Player* player) {
    if (!player) return;

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

bool loadPlayerMedia(Player* player) {
    // Right-facing images
    player->idle[DIR_RIGHT] = loadImage("player/images/droite/immobile.png");
    player->walk[DIR_RIGHT][0] = loadImage("player/images/droite/marcher1.png");
    player->walk[DIR_RIGHT][1] = loadImage("player/images/droite/marcher6.png");
    player->walk[DIR_RIGHT][2] = loadImage("player/images/droite/marcher4.png");
    player->run[DIR_RIGHT][0] = loadImage("player/images/droite/course1.png");
    player->run[DIR_RIGHT][1] = loadImage("player/images/droite/course2.png");
    player->run[DIR_RIGHT][2] = loadImage("player/images/droite/course3.png");
    player->crouch[DIR_RIGHT] = loadImage("player/images/droite/accroupi.png");
    player->down[DIR_RIGHT][0] = loadImage("player/images/droite/sol1.png");
    player->down[DIR_RIGHT][1] = loadImage("player/images/droite/sol2.png");
    player->attack[DIR_RIGHT][0] = loadImage("player/images/droite/attaque1.png");
    player->attack[DIR_RIGHT][1] = loadImage("player/images/droite/attaque2.png");

    // Left-facing images
    player->idle[DIR_LEFT] = loadImage("player/images/gauche/immobile.png");
    player->walk[DIR_LEFT][0] = loadImage("player/images/gauche/marcher1.png");
    player->walk[DIR_LEFT][1] = loadImage("player/images/gauche/marcher6.png");
    player->walk[DIR_LEFT][2] = loadImage("player/images/gauche/marcher4.png");
    player->run[DIR_LEFT][0] = loadImage("player/images/gauche/course1.png");
    player->run[DIR_LEFT][1] = loadImage("player/images/gauche/course2.png");
    player->run[DIR_LEFT][2] = loadImage("player/images/gauche/course3.png");
    player->crouch[DIR_LEFT] = loadImage("player/images/gauche/accroupi.png");
    player->down[DIR_LEFT][0] = loadImage("player/images/gauche/sol1.png");
    player->down[DIR_LEFT][1] = loadImage("player/images/gauche/sol2.png");
    player->attack[DIR_LEFT][0] = loadImage("player/images/gauche/attaque1.png");
    player->attack[DIR_LEFT][1] = loadImage("player/images/gauche/attaque2.png");

    player->jumpSurface[DIR_RIGHT] = loadImage("player/images/droite/jump.png");
    player->jumpSurface[DIR_LEFT] = loadImage("player/images/gauche/jump.png");

    return (player->idle[DIR_RIGHT] && player->idle[DIR_LEFT]);
}

void handlePlayerInput(Player* player, const Uint8* keystates) {
    if (!player) return;

    // Reset velocity
    player->velX = 0;

    // Handle movement
    if (keystates[SDLK_LEFT]) {
        player->velX = -PLAYER_WALK_SPEED;
        player->currentDir = DIR_LEFT;
        player->currentAction = ACTION_WALK;
    }
    if (keystates[SDLK_RIGHT]) {
        player->velX = PLAYER_WALK_SPEED;
        player->currentDir = DIR_RIGHT;
        player->currentAction = ACTION_WALK;
    }
    if (keystates[SDLK_LSHIFT] && (keystates[SDLK_LEFT] || keystates[SDLK_RIGHT])) {
        player->velX *= 2;
        player->currentAction = ACTION_RUN;
    }
    if (keystates[SDLK_DOWN]) {
        player->currentAction = ACTION_CROUCH;
    }
    if (keystates[SDLK_SPACE] && player->isOnGround) {
        player->velY = -JUMP_POWER;
        player->isJumping = true;
        player->isOnGround = false;
        player->currentAction = ACTION_JUMP;
    }
    if (keystates[SDLK_a]) {
        player->currentAction = ACTION_ATTACK;
        player->isAttacking = true;
    }

    // If no movement keys are pressed, return to idle
    if (!keystates[SDLK_LEFT] && !keystates[SDLK_RIGHT] && 
        !keystates[SDLK_DOWN] && !keystates[SDLK_SPACE] && 
        !keystates[SDLK_a] && player->isOnGround) {
        player->currentAction = ACTION_IDLE;
    }
}

void updatePlayer(Player* player) {
    if (!player) return;

    // Update position
    player->pos.x += player->velX;
    player->pos.y += player->velY;

    // Apply gravity
    if (!player->isOnGround) {
        player->velY += GRAVITY;
    }

    // Check ground collision
    if (player->pos.y >= GROUND_Y - player->pos.h) {
        player->pos.y = GROUND_Y - player->pos.h;
        player->velY = 0;
        player->isOnGround = true;
        player->isJumping = false;
    }

    // Update animation frame
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - player->lastFrameTime > ANIMATION_DELAY) {
        player->currentFrame = (player->currentFrame + 1) % WALK_FRAMES;
        player->lastFrameTime = currentTime;
    }

    // Reset attack state after animation
    if (player->isAttacking && currentTime - player->lastFrameTime > ANIMATION_DELAY * ATTACK_FRAMES) {
        player->isAttacking = false;
        player->currentAction = player->previousAction;
    }
}

void renderPlayer(Player* player, SDL_Surface* screen) {
    if (!player || !screen) return;

    SDL_Surface* currentSurface = NULL;
    SDL_Rect frameRect = {0, 0, player->pos.w, player->pos.h};

    switch (player->currentAction) {
        case ACTION_IDLE:
            currentSurface = player->idle[player->currentDir];
            break;
        case ACTION_WALK:
            currentSurface = player->walk[player->currentDir][player->currentFrame];
            break;
        case ACTION_RUN:
            currentSurface = player->run[player->currentDir][player->currentFrame];
            break;
        case ACTION_CROUCH:
            currentSurface = player->crouch[player->currentDir];
            break;
        case ACTION_JUMP:
            currentSurface = player->jumpSurface[player->currentDir];
            break;
        case ACTION_DOWN:
            currentSurface = player->down[player->currentDir][player->currentFrame % DOWN_FRAMES];
            break;
        case ACTION_ATTACK:
            currentSurface = player->attack[player->currentDir][player->currentFrame % ATTACK_FRAMES];
            break;
    }

    if (currentSurface) {
        SDL_BlitSurface(currentSurface, NULL, screen, &player->pos);
    }
}

// Background Functions
void initialiserBackground(Background *bg, const char *path, int xPosEcran) {
    bg->img = IMG_Load(path);
    if (!bg->img) {
        printf("Erreur de chargement de l'image: %s\n", path);
        exit(1);
    }
    bg->posEcran.x = xPosEcran;
    bg->posEcran.y = 0;
    bg->camera.x = 0;
    bg->camera.y = 0;
    bg->camera.w = bg->img->w;
    bg->camera.h = bg->img->h;
}

void afficherBackground(Background bg, SDL_Surface *ecran) {
    SDL_BlitSurface(bg.img, &bg.camera, ecran, &bg.posEcran);
}

// Text Functions
void afficherTexte(SDL_Surface *ecran, const char *texte, TTF_Font *police, int x, int y) {
    SDL_Color couleur = {255, 255, 255, 255};
    SDL_Surface *texteSurface = TTF_RenderText_Blended(police, texte, couleur);
    SDL_Rect pos = {x, y, 0, 0};
    SDL_BlitSurface(texteSurface, NULL, ecran, &pos);
    SDL_FreeSurface(texteSurface);
} 