#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

// Screen Dimensions
#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 875
#define SCREEN_W 1280
#define SCREEN_H 768

// Game Constants
#define NB_FEUILLes 10
#define NB_ETOILES 50
#define NB_COINS 14

// Player Constants
#define PLAYER_WALK_SPEED 4
#define PLAYER_RUN_SPEED 8
#define JUMP_POWER 15
#define GRAVITY 1
#define GROUND_Y (SCREEN_HEIGHT - 150)
#define JUMP_FRAMES 1
#define WALK_FRAMES 3
#define RUN_FRAMES 3
#define DOWN_FRAMES 2
#define ATTACK_FRAMES 2
#define ANIMATION_DELAY 100

// Game states
typedef enum {
    STATE_MENU,
    STATE_STAGE_ONE,
    STATE_QUIT
} GameState;

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

// Menu Structure
typedef struct {
    // Main menu
    SDL_Surface *Bg_principale;
    SDL_Surface *jouer1[2];
    SDL_Surface *option1[2];
    SDL_Surface *meilleurs1[2];
    SDL_Surface *historique1[2];
    SDL_Surface *quitter1[2];
    SDL_Rect pos_jouer1;
    SDL_Rect pos_option1;
    SDL_Rect pos_meilleurs1;
    SDL_Rect pos_historique1;
    SDL_Rect pos_quitter1;
    SDL_Rect positionTexte;
    int selection;
    Mix_Music *bg_music;
    TTF_Font *arial;
    SDL_Color text_color;
    SDL_Surface *saisie;
    SDL_Rect pos_saisie;
    char saisie_texte[20];

    // Options menu
    SDL_Surface *bg_option;
    SDL_Surface *option_augmenter[2];
    SDL_Surface *option_diminuer[2];
    SDL_Surface *option_normale[2];
    SDL_Surface *option_plein[2];
    SDL_Surface *retour[2];
    SDL_Rect pos_augmenter;
    SDL_Rect pos_diminuer;
    SDL_Rect pos_normale;
    SDL_Rect pos_plein;
    SDL_Rect pos_retour;
    int volume_music;
    int etat_plein_ecran;

    // Save/Load menu
    SDL_Surface *bg_sauvegarde_charg[2];
    SDL_Surface *bouton_oui[2];
    SDL_Surface *bouton_non[2];
    SDL_Surface *bouton_charger[2];
    SDL_Surface *bouton_nouvelle[2];
    SDL_Rect pos_bouton_oui;
    SDL_Rect pos_bouton_non;
    SDL_Rect pos_bouton_charger;
    SDL_Rect pos_bouton_nouvelle;
    int p_sauveg;

    // Player menu
    SDL_Surface *bg_M_joueur[2];
    SDL_Surface *bouton_mono[2];
    SDL_Surface *bouton_multi[2];
    SDL_Rect pos_bouton_mono;
    SDL_Rect pos_bouton_multi;
    int etat_joueur;

    // Player selection
    SDL_Surface *bouton_j1[2];
    SDL_Surface *bouton_j2[2];
    SDL_Surface *bouton_valider[2];
    SDL_Surface *bouton_retour_1[2];
    SDL_Rect pos_bouton_j1;
    SDL_Rect pos_bouton_j2;
    SDL_Rect pos_bouton_valider;
    SDL_Rect pos_bouton_retour_1;

    // High scores
    SDL_Surface *bg_meilleur_s[2];
    SDL_Surface *bouton_valider_1[2];
    SDL_Rect pos_bouton_valider_1;
    SDL_Surface *bouton_quitter_2[2];
    SDL_Surface *bouton_retour_2[2];
    SDL_Rect pos_bouton_retour_2;
    SDL_Rect pos_bouton_quitter_2;
} Menu;

// Background Structure
typedef struct {
    SDL_Surface *img;
    SDL_Rect posEcran;
    SDL_Rect camera;
} Background;

// Leaf Structure
typedef struct {
    SDL_Surface *img;
    SDL_Rect pos;
    int vitesse;
    int xOrigine;
} Feuille;

// Star Structure
typedef struct {
    SDL_Rect pos;
    float phase;
    float vitesse;
    int luminosite;
} Etoile;

// Coin Structure
typedef struct {
    SDL_Surface *img;
    SDL_Rect pos;
} Coin;

// Function Declarations
void init_menu(Menu *M);
void Afficher_menu(Menu M, SDL_Surface *ecran);
int selectionner_bouton(SDL_Rect pos_bouton);
void miseajour(Menu *M);
void Afficher_menu_option(Menu M, SDL_Surface *ecran);
void Afficher_menu_Sauvegarde(Menu M, SDL_Surface *ecran);
void Afficher_chargement(Menu M, SDL_Surface *ecran);
void Afficher_Joueur(Menu M, SDL_Surface *ecran);
void Afficher_Choisir_Joueur(Menu M, SDL_Surface *ecran);
void Afficher_Meilleur_score(Menu M, SDL_Surface *ecran);
void Afficher_liste(Menu M, SDL_Surface *ecran);

void initialiserBackground(Background *bg, const char *path, int xPosEcran);
void afficherBackground(Background bg, SDL_Surface *ecran);
void scrollingArrows(Background *bg, const Uint8 *keys);
void scrollingKeys(Background *bg, const Uint8 *keys);
void initialiserFeuille(Feuille *f, SDL_Surface *img, int x, int y, int vitesse);
void animerFeuille(Feuille *f, int hauteurEcran, SDL_Surface *feuille1, SDL_Surface *feuille2);
void afficherFeuille(Feuille f, SDL_Surface *ecran);
void initialiserEtoiles(Etoile *etoiles, int nbEtoiles, SDL_Surface *backgroundImage);
void animerEtoiles(Etoile *etoiles, int nbEtoiles, float temps);
void afficherEtoiles(Etoile *etoiles, int nbEtoiles, SDL_Surface *ecran, SDL_Surface *etoileSurface, SDL_Rect *camera);
void afficherCoins(Coin *coins, int nbCoins, SDL_Surface *ecran, SDL_Rect *camera, int offsetX);

// Player Functions
SDL_Surface* loadImage(const char* filename) {
    SDL_Surface* loadedImage = IMG_Load(filename);
    if (!loadedImage) {
        printf("Failed to load image %s: %s\n", filename, IMG_GetError());
        return NULL;
    }
    SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage);
    return optimizedImage;
}

Player* createPlayer(int startX, int startY) {
    Player* player = (Player*)malloc(sizeof(Player));
    if (!player) return NULL;

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

    // Initialize all surfaces to NULL
    for (int i = 0; i < 2; i++) {
        player->idle[i] = NULL;
        player->crouch[i] = NULL;
        player->jumpSurface[i] = NULL;
        for (int j = 0; j < WALK_FRAMES; j++) player->walk[i][j] = NULL;
        for (int j = 0; j < RUN_FRAMES; j++) player->run[i][j] = NULL;
        for (int j = 0; j < DOWN_FRAMES; j++) player->down[i][j] = NULL;
        for (int j = 0; j < ATTACK_FRAMES; j++) player->attack[i][j] = NULL;
    }

    // Load player sprites
    player->idle[DIR_RIGHT] = loadImage("player/images/droite/immobile.png");
    player->idle[DIR_LEFT] = loadImage("player/images/gauche/immobile.png");
    
    // Load walk animations
    for (int i = 0; i < WALK_FRAMES; i++) {
        char filename[100];
        snprintf(filename, sizeof(filename), "player/images/droite/marcher%d.png", i + 1);
        player->walk[DIR_RIGHT][i] = loadImage(filename);
        snprintf(filename, sizeof(filename), "player/images/gauche/marcher%d.png", i + 1);
        player->walk[DIR_LEFT][i] = loadImage(filename);
    }

    // Load jump sprites
    player->jumpSurface[DIR_RIGHT] = loadImage("player/images/droite/jump.png");
    player->jumpSurface[DIR_LEFT] = loadImage("player/images/gauche/jump.png");

    // Set initial dimensions
    if (player->idle[DIR_RIGHT]) {
        player->pos.w = player->idle[DIR_RIGHT]->w;
        player->pos.h = player->idle[DIR_RIGHT]->h;
        player->pos.y = GROUND_Y - player->pos.h;
    }

    return player;
}

void destroyPlayer(Player* player) {
    if (!player) return;
    
    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(player->idle[i]);
        SDL_FreeSurface(player->crouch[i]);
        SDL_FreeSurface(player->jumpSurface[i]);
        for (int j = 0; j < WALK_FRAMES; j++) SDL_FreeSurface(player->walk[i][j]);
        for (int j = 0; j < RUN_FRAMES; j++) SDL_FreeSurface(player->run[i][j]);
        for (int j = 0; j < DOWN_FRAMES; j++) SDL_FreeSurface(player->down[i][j]);
        for (int j = 0; j < ATTACK_FRAMES; j++) SDL_FreeSurface(player->attack[i][j]);
    }
    
    free(player);
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
    else if (keystates[SDLK_RIGHT]) {
        player->velX = PLAYER_WALK_SPEED;
        player->currentDir = DIR_RIGHT;
        player->currentAction = ACTION_WALK;
    }
    else {
        player->currentAction = ACTION_IDLE;
    }

    // Handle running
    if (keystates[SDLK_LSHIFT] && (keystates[SDLK_LEFT] || keystates[SDLK_RIGHT])) {
        player->velX *= 2;
        player->currentAction = ACTION_RUN;
    }

    // Handle jumping
    if (keystates[SDLK_SPACE] && player->isOnGround) {
        player->velY = -JUMP_POWER;
        player->isJumping = true;
        player->isOnGround = false;
        player->currentAction = ACTION_JUMP;
    }

    // Handle crouching
    if (keystates[SDLK_DOWN]) {
        player->currentAction = ACTION_CROUCH;
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

    // Ground collision
    if (player->pos.y > GROUND_Y - player->pos.h) {
        player->pos.y = GROUND_Y - player->pos.h;
        player->velY = 0;
        player->isOnGround = true;
        player->isJumping = false;
        if (player->currentAction == ACTION_JUMP) {
            player->currentAction = player->previousAction;
        }
    }

    // Screen boundaries
    if (player->pos.x < 0) player->pos.x = 0;
    if (player->pos.x > SCREEN_WIDTH - player->pos.w) {
        player->pos.x = SCREEN_WIDTH - player->pos.w;
    }

    // Animation timing
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - player->lastFrameTime > ANIMATION_DELAY) {
        player->currentFrame = (player->currentFrame + 1) % WALK_FRAMES;
        player->lastFrameTime = currentTime;
    }
}

void renderPlayer(Player* player, SDL_Surface* screen) {
    if (!player || !screen) return;

    SDL_Surface* currentSprite = NULL;
    
    switch (player->currentAction) {
        case ACTION_IDLE:
            currentSprite = player->idle[player->currentDir];
            break;
        case ACTION_WALK:
            currentSprite = player->walk[player->currentDir][player->currentFrame];
            break;
        case ACTION_RUN:
            currentSprite = player->run[player->currentDir][player->currentFrame];
            break;
        case ACTION_JUMP:
            currentSprite = player->jumpSurface[player->currentDir];
            break;
        case ACTION_CROUCH:
            currentSprite = player->crouch[player->currentDir];
            break;
        default:
            currentSprite = player->idle[player->currentDir];
    }

    if (currentSprite) {
        SDL_BlitSurface(currentSprite, NULL, screen, &player->pos);
    }
}

// Menu Implementation
void init_menu(Menu *M) {
    M->Bg_principale = IMG_Load("menu/Bg_principale.png");
    if (M->Bg_principale == NULL) {
        printf("Erreur chargement image : %s\n", IMG_GetError());
    }

    // Music
    M->bg_music = Mix_LoadMUS("menu/SB.mp3");
    Mix_PlayMusic(M->bg_music, -1);

    // Load menu buttons
    M->jouer1[0] = IMG_Load("menu/jouer1.png");
    M->jouer1[1] = IMG_Load("menu/jouer2.png");
    M->option1[0] = IMG_Load("menu/option1.png");
    M->option1[1] = IMG_Load("menu/option2.png");
    M->meilleurs1[0] = IMG_Load("menu/meilleurs1.png");
    M->meilleurs1[1] = IMG_Load("menu/meilleurs2.png");
    M->historique1[0] = IMG_Load("menu/historique1.png");
    M->historique1[1] = IMG_Load("menu/historique2.png");
    M->quitter1[0] = IMG_Load("menu/quitter1.png");
    M->quitter1[1] = IMG_Load("menu/quitter2.png");

    // Set button positions
    M->pos_jouer1.x = 330;
    M->pos_jouer1.y = 175;
    M->pos_jouer1.w = M->jouer1[0]->w;
    M->pos_jouer1.h = M->jouer1[0]->h;

    M->pos_option1.x = 330;
    M->pos_option1.y = 285;
    M->pos_option1.w = M->option1[0]->w;
    M->pos_option1.h = M->option1[0]->h;

    M->pos_meilleurs1.x = 330;
    M->pos_meilleurs1.y = 393;
    M->pos_meilleurs1.w = M->meilleurs1[0]->w;
    M->pos_meilleurs1.h = M->meilleurs1[0]->h;

    M->pos_historique1.x = 330;
    M->pos_historique1.y = 500;
    M->pos_historique1.w = M->historique1[0]->w;
    M->pos_historique1.h = M->historique1[0]->h;

    M->pos_quitter1.x = 775;
    M->pos_quitter1.y = 500;
    M->pos_quitter1.w = M->quitter1[0]->w;
    M->pos_quitter1.h = M->quitter1[0]->h;

    // Initialize other menu components
    M->volume_music = 50;
    Mix_VolumeMusic(M->volume_music);
    M->etat_plein_ecran = 0;
    M->p_sauveg = 0;
    M->etat_joueur = 0;

    // Load font
    M->arial = TTF_OpenFont("menu/arial.ttf", 38);
    if (M->arial == NULL) {
        printf("Erreur chargement font : %s\n", TTF_GetError());
    }

    M->text_color.r = 0;
    M->text_color.b = 0;
    M->text_color.g = 0;

    M->pos_saisie.x = 490;
    M->pos_saisie.y = 305;
}

void Afficher_menu(Menu M, SDL_Surface *ecran) {
    SDL_BlitSurface(M.Bg_principale, NULL, ecran, NULL);

    if (selectionner_bouton(M.pos_jouer1) == 0) {
        SDL_BlitSurface(M.jouer1[0], NULL, ecran, &M.pos_jouer1);
    } else {
        SDL_BlitSurface(M.jouer1[1], NULL, ecran, &M.pos_jouer1);
    }

    if (selectionner_bouton(M.pos_option1) == 0) {
        SDL_BlitSurface(M.option1[0], NULL, ecran, &M.pos_option1);
    } else {
        SDL_BlitSurface(M.option1[1], NULL, ecran, &M.pos_option1);
    }

    if (selectionner_bouton(M.pos_meilleurs1) == 0) {
        SDL_BlitSurface(M.meilleurs1[0], NULL, ecran, &M.pos_meilleurs1);
    } else {
        SDL_BlitSurface(M.meilleurs1[1], NULL, ecran, &M.pos_meilleurs1);
    }

    if (selectionner_bouton(M.pos_historique1) == 0) {
        SDL_BlitSurface(M.historique1[0], NULL, ecran, &M.pos_historique1);
    } else {
        SDL_BlitSurface(M.historique1[1], NULL, ecran, &M.pos_historique1);
    }

    if (selectionner_bouton(M.pos_quitter1) == 0) {
        SDL_BlitSurface(M.quitter1[0], NULL, ecran, &M.pos_quitter1);
    } else {
        SDL_BlitSurface(M.quitter1[1], NULL, ecran, &M.pos_quitter1);
    }
}

int selectionner_bouton(SDL_Rect pos_bouton) {
    int x_mouse, y_mouse;
    SDL_GetMouseState(&x_mouse, &y_mouse);

    if ((x_mouse > pos_bouton.x) && (x_mouse < (pos_bouton.x + pos_bouton.w)) &&
        (y_mouse > pos_bouton.y) && (y_mouse < (pos_bouton.y + pos_bouton.h))) {
        return 1;
    }
    return 0;
}

void miseajour(Menu *M) {
    if (selectionner_bouton(M->pos_jouer1) == 1) {
        M->selection = 1;
    } else if (selectionner_bouton(M->pos_option1) == 1) {
        M->selection = 2;
    } else if (selectionner_bouton(M->pos_meilleurs1) == 1) {
        M->selection = 3;
    } else if (selectionner_bouton(M->pos_historique1) == 1) {
        M->selection = 4;
    } else if (selectionner_bouton(M->pos_quitter1) == 1) {
        M->selection = 5;
    } else {
        M->selection = 0;
    }
}

// Stage One Implementation
void initialiserBackground(Background *bg, const char *path, int xPosEcran) {
    bg->img = IMG_Load(path);
    if (!bg->img) {
        printf("Erreur de chargement image: %s\n", IMG_GetError());
        return;
    }

    bg->posEcran.x = xPosEcran;
    bg->posEcran.y = 0;

    bg->camera.x = 0;
    bg->camera.y = 0;
    bg->camera.w = 640;
    bg->camera.h = 800;
}

void scrollingArrows(Background *bg, const Uint8 *keys) {
    if (keys[SDLK_UP]) bg->camera.y -= 10;
    if (keys[SDLK_DOWN]) bg->camera.y += 10;
    if (keys[SDLK_LEFT]) bg->camera.x -= 10;
    if (keys[SDLK_RIGHT]) bg->camera.x += 10;

    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.x > bg->img->w - bg->camera.w) bg->camera.x = bg->img->w - bg->camera.w;
    if (bg->camera.y > bg->img->h - bg->camera.h) bg->camera.y = bg->img->h - bg->camera.h;
}

void initialiserFeuille(Feuille *f, SDL_Surface *img, int x, int y, int vitesse) {
    f->img = img;
    f->pos.x = x;
    f->pos.y = y;
    f->vitesse = vitesse;
    f->xOrigine = x;
}

void animerFeuille(Feuille *f, int hauteurEcran, SDL_Surface *feuille1, SDL_Surface *feuille2) {
    f->pos.y += f->vitesse;

    if (f->pos.y > hauteurEcran) {
        f->pos.y = -f->img->h;
        f->xOrigine = rand() % (1800 - f->img->w);
        f->img = (rand() % 2 == 0) ? feuille1 : feuille2;
    }

    f->pos.x = f->xOrigine + sin(f->pos.y * 0.01) * 20;
}

void afficherFeuille(Feuille f, SDL_Surface *ecran) {
    SDL_Rect dst = f.pos;
    SDL_BlitSurface(f.img, NULL, ecran, &dst);
}

void initialiserEtoiles(Etoile *etoiles, int nbEtoiles, SDL_Surface *backgroundImage) {
    for (int i = 0; i < nbEtoiles; i++) {
        etoiles[i].pos.x = rand() % backgroundImage->w;
        etoiles[i].pos.y = rand() % (backgroundImage->h / 2);
        etoiles[i].phase = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
        etoiles[i].vitesse = (float)(rand() % 3 + 1) / 100.0f;
        etoiles[i].luminosite = 0;
    }
}

void animerEtoiles(Etoile *etoiles, int nbEtoiles, float temps) {
    for (int i = 0; i < nbEtoiles; i++) {
        float alpha = (sin(temps * etoiles[i].vitesse + etoiles[i].phase) + 1.0f) * 127.5f;
        etoiles[i].luminosite = (int)alpha;
    }
}

void afficherEtoiles(Etoile *etoiles, int nbEtoiles, SDL_Surface *ecran, SDL_Surface *etoileSurface, SDL_Rect *camera) {
    for (int i = 0; i < nbEtoiles; i++) {
        SDL_Rect dst = etoiles[i].pos;
        dst.x -= camera->x;
        dst.y -= camera->y;

        Uint8 alpha = (Uint8)etoiles[i].luminosite;
        SDL_SetAlpha(etoileSurface, SDL_SRCALPHA, alpha);
        SDL_BlitSurface(etoileSurface, NULL, ecran, &dst);
    }
}

void afficherCoins(Coin *coins, int nbCoins, SDL_Surface *ecran, SDL_Rect *camera, int offsetX) {
    for (int i = 0; i < nbCoins; i++) {
        int xScreen = coins[i].pos.x - camera->x + offsetX;
        int yScreen = coins[i].pos.y - camera->y;

        if (xScreen + coins[i].pos.w >= offsetX &&
            xScreen <= offsetX + camera->w &&
            yScreen + coins[i].pos.h >= 0 &&
            yScreen <= ecran->h) {

            SDL_Rect dst = {xScreen, yScreen, coins[i].pos.w, coins[i].pos.h};
            SDL_BlitSurface(coins[i].img, NULL, ecran, &dst);
        }
    }
}

// Main function
int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    SDL_Surface *ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Jeu", NULL);

    GameState currentState = STATE_MENU;
    SDL_Event event;
    int quit = 0;

    // Initialize menu
    Menu menu;
    init_menu(&menu);

    // Initialize stage one variables
    Background bg1, bg2;
    int soloInit = 0, multiInit = 0;
    SDL_Surface *feuille1 = NULL, *feuille2 = NULL;
    SDL_Surface *etoileSurface = NULL;
    Feuille feuilles[NB_FEUILLes];
    Etoile etoiles[NB_ETOILES];
    Coin coins[NB_COINS];

    // Initialize player
    Player* player = createPlayer(SCREEN_WIDTH / 2, GROUND_Y);
    if (!player) {
        printf("Failed to create player\n");
        return 1;
    }

    while (!quit) {
        switch (currentState) {
            case STATE_MENU: {
                Afficher_menu(menu, ecran);
                SDL_Flip(ecran);

                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            quit = 1;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) {
                                if (menu.selection == 5) {
                                    quit = 1;
                                } else if (menu.selection == 1) {
                                    currentState = STATE_STAGE_ONE;
                                    
                                    // Initialize stage one resources
                                    feuille1 = IMG_Load("stage .1/feuille1.png");
                                    feuille2 = IMG_Load("stage .1/feuille2.png");
                                    etoileSurface = IMG_Load("stage .1/etoile.png");
                                    
                                    if (!feuille1 || !feuille2 || !etoileSurface) {
                                        printf("Error loading images: %s\n", IMG_GetError());
                                        quit = 1;
                                        break;
                                    }
                                    
                                    // Initialize leaves
                                    srand(time(NULL));
                                    for (int i = 0; i < NB_FEUILLes; i++) {
                                        int x = rand() % (1800 - feuille1->w);
                                        int y = -(rand() % 500);
                                        int vitesse = 2 + rand() % 4;
                                        SDL_Surface *imgChoisie = (rand() % 2 == 0) ? feuille1 : feuille2;
                                        initialiserFeuille(&feuilles[i], imgChoisie, x, y, vitesse);
                                    }

                                    // Initialize backgrounds
                                    initialiserBackground(&bg1, "stage .1/bg1.png", 0);
                                    initialiserBackground(&bg2, "stage .1/bg1.png", ecran->w / 2);
                                    
                                    // Initialize stars
                                    initialiserEtoiles(etoiles, NB_ETOILES, bg1.img);
                                    
                                    // Initialize coins
                                    SDL_Surface *argent = IMG_Load("stage .1/argent.png");
                                    if (!argent) {
                                        printf("Error loading argent.png: %s\n", IMG_GetError());
                                        quit = 1;
                                        break;
                                    }
                                    int posX[NB_COINS] = {600, 2250, 2450, 3000, 2800, 3100, 3850, 4400, 4900, 5450, 5680, 6200, 7310, 8100};
                                    int posY[NB_COINS] = {240, 655, 655, 240, 240, 720, 380, 775, 540, 620, 620, 275, 440, 390};
                                    for (int i = 0; i < NB_COINS; i++) {
                                        coins[i].img = argent;
                                        coins[i].pos.x = posX[i];
                                        coins[i].pos.y = posY[i];
                                        coins[i].pos.w = argent->w;
                                        coins[i].pos.h = argent->h;
                                    }
                                }
                            }
                            break;
                    }
                }
                miseajour(&menu);
                break;
            }

            case STATE_STAGE_ONE: {
                if (!soloInit) {
                    bg1.camera.x = 0;
                    bg1.camera.y = 150;
                    bg1.camera.w = ecran->w;
                    bg1.camera.h = ecran->h;
                    soloInit = 1;
                }

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = 1;
                    }
                }

                const Uint8* keys = SDL_GetKeyState(NULL);
                scrollingArrows(&bg1, keys);
                handlePlayerInput(player, keys);
                updatePlayer(player);

                SDL_BlitSurface(bg1.img, &bg1.camera, ecran, &bg1.posEcran);
                
                for (int i = 0; i < NB_FEUILLes; i++) {
                    animerFeuille(&feuilles[i], 875, feuille1, feuille2);
                    afficherFeuille(feuilles[i], ecran);
                }

                float temps = (SDL_GetTicks() - SDL_GetTicks()) / 1000.0f;
                animerEtoiles(etoiles, NB_ETOILES, temps);
                afficherEtoiles(etoiles, NB_ETOILES, ecran, etoileSurface, &bg1.camera);

                afficherCoins(coins, NB_COINS, ecran, &bg1.camera, bg1.posEcran.x);

                // Render player
                renderPlayer(player, ecran);

                SDL_Flip(ecran);
                break;
            }
        }

        SDL_Delay(16);
    }

    // Cleanup
    destroyPlayer(player);
    SDL_FreeSurface(feuille1);
    SDL_FreeSurface(feuille2);
    SDL_FreeSurface(etoileSurface);
    SDL_FreeSurface(bg1.img);
    SDL_FreeSurface(bg2.img);
    SDL_FreeSurface(ecran);

    SDL_Quit();
    TTF_Quit();
    Mix_CloseAudio();

    return 0;
} 