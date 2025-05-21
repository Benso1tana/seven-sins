#include "bouton.h"

// Initialiser la mini-map avec image et position fixe
void initMiniMap(MiniMap *map, int level) {
    map->scale = 0.1f; // 10% de l’échelle du jeu principal
    map->level = level;

    // Chargement de l'image de la mini-map selon le niveau
    if (level == 1) {
        map->image = IMG_Load("minimap_level1.png");
    } else if (level == 2) {
        map->image = IMG_Load("minimap_level2.png");
    } else {
        fprintf(stderr, "Niveau inconnu: %d\n", level);
        exit(EXIT_FAILURE);
    }

    // Vérification du chargement de l'image de la mini-map
    if (!map->image) {
        fprintf(stderr, "Erreur de chargement de la mini-map: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Chargement de l'image du joueur sur la mini-map
    map->playerImage = IMG_Load("red.jpg");
    if (!map->playerImage) {
        fprintf(stderr, "Erreur de chargement de l’image du joueur (red.jpg): %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Position fixe de la mini-map à l'écran
    map->position.x = 100;
    map->position.y = 55;
    map->position.w = map->image->w;
    map->position.h = map->image->h;

    // Initialisation de la taille de l’icône du joueur
    map->playerIcon.w = map->playerImage->w;
    map->playerIcon.h = map->playerImage->h;
}

// Mise à jour basique de la mini-map (sans caméra)
void updateMiniMap(MiniMap *map, SDL_Rect playerPosition) {
    map->playerIcon.x = (int)(playerPosition.x * map->scale);
    map->playerIcon.y = (int)(playerPosition.y * map->scale);
}

// Affiche l’image de la mini-map
void renderMiniMap(SDL_Surface *screen, MiniMap *map) {
    SDL_BlitSurface(map->image, NULL, screen, &map->position);
}

// Dessine l’icône du joueur sur la mini-map
void drawPlayerOnMiniMap(SDL_Surface *screen, MiniMap *map, SDL_Rect playerPosition) {
    SDL_Rect screenPos;
    screenPos.x = map->position.x + (int)(playerPosition.x * map->scale + 20);
    screenPos.y = map->position.y + (int)(playerPosition.y * map->scale + 70);
    screenPos.w = map->playerImage->w;
    screenPos.h = map->playerImage->h;

    SDL_BlitSurface(map->playerImage, NULL, screen, &screenPos);
}

// Collision pixel perfect (avec couleur rouge)
int checkCollisionPerfectPixel(SDL_Surface *surface, SDL_Rect *player) {
    if (!surface || !player) return 0;

    if (player->x < 0 || player->x >= surface->w || 
        player->y < 0 || player->y >= surface->h) {
        return 0;
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 pixel = pixels[player->y * surface->w + player->x];
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

    return (r == 255 && g == 0 && b == 0); // Rouge pur
}

// Collision par boîte englobante
int checkCollisionBB(SDL_Rect *a, SDL_Rect *b) {
    return (a->x < b->x + b->w &&
            a->x + a->w > b->x &&
            a->y < b->y + b->h &&
            a->y + a->h > b->y);
}

// Mise à jour de la position du joueur sur la mini-map (avec caméra)
void MAJMinimap(SDL_Rect posJoueur, MiniMap *m, SDL_Rect camera, int redimensionnement) {
    SDL_Rect posJoueurABS;
    posJoueurABS.x = posJoueur.x + camera.x;
    posJoueurABS.y = posJoueur.y + camera.y;

    m->playerIcon.x = posJoueurABS.x * redimensionnement / 100;
    m->playerIcon.y = posJoueurABS.y * redimensionnement / 100;
}

// Affichage complet mini-map + icône joueur
void afficher(MiniMap m, SDL_Surface *screen) {
    SDL_BlitSurface(m.image, NULL, screen, &m.position);

    SDL_Rect playerPosOnMap = {
        m.playerIcon.x,
        m.playerIcon.y,
        m.playerIcon.w,
        m.playerIcon.h
    };

    SDL_BlitSurface(m.playerImage, NULL, screen, &playerPosOnMap);
}

// Libération de la mémoire
void Liberer(MiniMap *m) {
    if (m->image) SDL_FreeSurface(m->image);
    if (m->playerImage) SDL_FreeSurface(m->playerImage);
}
void save_game(int x_player, int y_player, int cam_x, int cam_y, int score, int lives) {
    FILE *f = fopen("save.txt", "w");
    if (f == NULL) {
        printf("Erreur lors de la sauvegarde.\n");
        return;
    }

    fprintf(f, "%d %d %d %d %d %d\n", x_player, y_player, cam_x, cam_y, score, lives);
    fclose(f);
}

void load_game(int *x_player, int *y_player, int *cam_x, int *cam_y, int *score, int *lives) {
    FILE *f = fopen("save.txt", "r");
    if (f == NULL) {
        printf("Aucune sauvegarde trouvée.\n");
        return;
    }

    fscanf(f, "%d %d %d %d %d %d", x_player, y_player, cam_x, cam_y, score, lives);
    fclose(f);
}



