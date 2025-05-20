#ifndef GAME_H
#define GAME_H

// --- Includes needed by multiple parts ---
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "puzzle.h"

// --- Platform Specific Serial Includes ---
#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

// --- *** ORDRE IMPORTANT: Constantes FONDAMENTALES (tailles tableaux, etc.) *** ---
#define MAX_ENEMIES          5 // Doit être avant la struct Game
#define NB_ACTIONS           5 // Doit être avant la struct Player
#define NB_FRAMES            4 // Doit être avant la struct Player
#define LIFE_STATES          3 // Doit être avant la struct Player


// --- Autres Constantes ---
#define SCREEN_WIDTH        2000
#define SCREEN_HEIGHT       1080
#define WORLD_WIDTH         8000
#define WORLD_FLOOR_Y       850
// Camera
#define INITIAL_CAMERA_Y      0
#define CAMERA_FOLLOW_MARGIN_X 400
#define CAMERA_FOLLOW_MARGIN_Y 200
// Background & Misc
#define MOBILE_PLATFORM_SPEED 2
#define TIMER_FONT_SIZE      24
#define TEXT_DISPLAY_TIME  11000
// Enemy
#define ENNEMI_HEALTH_MAX 3
// #define MAX_ENEMIES          5 // Déplacé plus haut
#define ANIM_MOVE 0
#define ANIM_SHOOT 1
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define ENNEMY_ATTACK_RANGE 300
#define ENNEMY_BULLET_SPEED 4
#define ENEMY_SPAWN_Y        600
#define ENEMY_HEALTHBAR_Y_OFFSET -20
#define ENEMY_HEART_SPACING 5
// Player
#define PLAYER_SPEED         8.0
#define GRAVITY              0.7f
#define JUMP_VELOCITY      -40.0f
#define MAX_FALL_SPEED      18.0f
#define PLAYER_BULLET_SPEED  10.0f
#define PLAYER_ATTACK_COOLDOWN 500
#define PLAYER_FRAME_INTERVAL 100
#define PLAYER_ATTACK_FRAME_INTERVAL 150
// #define NB_ACTIONS 5 // Déplacé plus haut
// #define NB_FRAMES 4 // Déplacé plus haut
// #define LIFE_STATES 3 // Déplacé plus haut
#define ANIM_WALK_RIGHT 0
#define ANIM_WALK_LEFT  1
#define ANIM_ATTACK     2
#define ANIM_JUMP       3
#define ANIM_IDLE       4
// Minimap
#define MINIMAP_PLAYER_ICON "joueur_minimap.png"
#define MINIMAP_BG_L1       "minimap_bg_scaled_L1.png"
#define MINIMAP_BG_L2       "minimap_bg_scaled_L2.png"
#define MINIMAP_SCREEN_X_OFFSET 20
#define MINIMAP_SCREEN_Y_OFFSET 100
#define MINIMAP_ENEMY_DOT_SIZE 5
#define MINIMAP_OBSTACLE_FLASH_INTERVAL 200
#define MINIMAP_SHAKE_AMPLITUDE 3
// Resource Files
#define PLAYER_BULLET_IMAGE_FILE "balle1.png"
#define ENEMY_BULLET_IMAGE_FILE "enemy_bullet.png"
#define HEART_FULL_FILE   "heart_full.png"
#define HEART_EMPTY_FILE  "heart_empty.png"
// Arduino Communication
#define ARDUINO_BUFFER_SIZE 64

// --- Enums ---
// [ ... Enums inchangées (PlatformType, GameState) ... ]
typedef enum { PLATFORM_FIXED, PLATFORM_MOBILE_H, ITEM_HOLE, PLATFORM_PUZZLE_TRIGGER } PlatformType;
typedef enum { STATE_RUNNING, STATE_PUZZLE, STATE_PAUSED, STATE_GAMEOVER } GameState;

// --- Structures (Ordre correct) ---
// [ ... Structures inchangées (GameTimer, GameText, Platform, Background, Animation, Projectile, Ennemi, Player, MinimapData, ArduinoInputState) ... ]
typedef struct { Uint32 start_ticks; TTF_Font *font; SDL_Surface *time_surface; Uint32 total_time; } GameTimer;
typedef struct { SDL_Surface *surface; SDL_Rect position; char text[128]; int active; int is_showing; Uint32 show_time; } GameText;
typedef struct { SDL_Surface *image; SDL_Rect position; PlatformType type; int move_range; int move_direction; int original_x; int original_y; } Platform;
typedef struct { SDL_Surface *bg_image; SDL_Rect camera; Platform *platforms; int platform_count; SDL_Surface *hole_img; GameTimer timer; GameText texts[3]; int level; int show_final_time; } Background;
typedef struct { SDL_Surface **frames; int frame_actuelle; int nb_frames; int delai_frame; int compteur_delai; int direction; } Animation;
typedef struct { SDL_Rect pos; float real_x; float real_y; float vx; float vy; int active; SDL_Surface* image; } Projectile;
typedef struct { SDL_Rect position; int vitesse_x; int direction; int health; Animation **animations; int est_en_attaque; int temps_changement_direction; int compteur_temps; int shoot_timer; int shoot_delay; } Ennemi;
typedef struct { SDL_Surface *images[NB_ACTIONS][NB_FRAMES]; SDL_Surface *life_images[LIFE_STATES]; SDL_Rect pos; float vx; float vy; int on_ground; int direction; int current_action; int current_frame; Uint32 last_frame_time; int life_state; int attacking; Uint32 last_attack_time; } Player; // NB_ACTIONS, NB_FRAMES, LIFE_STATES sont maintenant connus
typedef struct { SDL_Surface *minimap_bg_img; SDL_Surface *player_icon_img; SDL_Rect minimap_screen_pos; SDL_Rect player_icon_minimap_pos; float scale_x; float scale_y; } MinimapData;
typedef struct { int is_left_pressed; int is_right_pressed; int jump_pressed; int attack_pressed; int settings_pressed; int center_pressed; } ArduinoInputState;


// --- Structure Principale du Jeu ---
typedef struct {
    SDL_Surface *screen;
    int running; int current_level; Uint32 start_time;
    Background background; Player player;
    Ennemi *enemies[MAX_ENEMIES]; // MAX_ENEMIES est maintenant connu
    Projectile enemy_bullet; Projectile player_bullet;
    Uint8 *keystate; SDL_Surface *heart_full_img; SDL_Surface *heart_empty_img; MinimapData minimap;
    Uint32 obstacle_flash_timer; int current_obstacle_color_index; int is_minimap_shaking;
    GameState game_state; PuzzleGame puzzle_instance; TTF_Font* puzzle_font;
#ifdef _WIN32
    HANDLE serial_handle;
#else
    int serial_fd;
#endif
    char arduino_buffer[ARDUINO_BUFFER_SIZE];
    ArduinoInputState arduino_state;
} Game;


// --- Prototypes de Fonctions ---
// [ ... Prototypes inchangés ... ]
int init_game(Game *game, int initial_level, const char *serial_port);
void cleanup_game(Game *game);
void handle_input(Game *game);
void update_game(Game *game);
void render_game(Game *game);
int init_arduino_input(Game *game, const char *port_name);
void cleanup_arduino_input(Game *game);
void read_and_parse_arduino(Game *game);
void send_to_arduino(Game *game, const char *data);
void change_level(Game *game, int new_level);
void init_background(Background *bg, int level, Uint32 start_ticks);
void free_background(Background *bg);
void update_platforms(Background *bg);
void update_camera(Background *bg, SDL_Rect player_pos);
void update_timer(GameTimer *timer);
void render_all(Background *bg, SDL_Surface *screen);
Ennemi* initialiser_ennemi(int niveau, int start_x, int start_y);
void liberer_ennemi(Ennemi *ennemi);
void deplacer_ennemi(Ennemi *ennemi, SDL_Rect limites, SDL_Rect player_pos);
void animer_ennemi(Ennemi *ennemi);
void ennemi_shoot(Ennemi *ennemi, Projectile *bullet, SDL_Rect player_pos);
int detecter_collision_joueur_ennemi(SDL_Rect pos_joueur, Ennemi *ennemi);
void gerer_sante_ennemi(Ennemi *ennemi, int dommage);
void afficher_ennemi_on_screen(Ennemi *ennemi, SDL_Surface *ecran, SDL_Rect camera);
int init_player(Player *p);
void free_player(Player *p);
void update_player_animation(Player *p);
void render_player(Player *p, SDL_Surface *screen, SDL_Rect camera);
void render_player_hud(Player *p, SDL_Surface *screen);
void player_take_damage(Player *p, int amount);
void player_gain_life(Player *p);
void player_shoot(Game *game);
int init_minimap(MinimapData *mm);
int load_minimap_for_level(MinimapData *mm, Background *bg, int level_num);
void free_minimap(MinimapData *mm);
void update_minimap(Game *game);
void render_minimap(Game *game);
PuzzleState run_puzzle(Game *game);
int check_collision(SDL_Rect a, SDL_Rect b);

#endif // GAME_H
