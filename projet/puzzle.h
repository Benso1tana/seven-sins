#ifndef PUZZLE_H
#define PUZZLE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <SDL/SDL_mixer.h>

#define PUZZLE_SIZE 3
#define TIMER_DURATION 60 // Secondes

typedef enum {
    PUZZLE_STATE_RUNNING,
    PUZZLE_STATE_WON,
    PUZZLE_STATE_LOST
} PuzzleState;

// Structure Point pour SDL 1.2
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Mix_Music* background_music;
    SDL_Surface* background;
    bool piece_locked[PUZZLE_SIZE * PUZZLE_SIZE];
    bool need_redraw;
    int selected_piece;
    Point drag_offset;
    SDL_Surface* pieces[PUZZLE_SIZE * PUZZLE_SIZE];
    SDL_Rect positions[PUZZLE_SIZE * PUZZLE_SIZE];
    SDL_Rect correct_positions[PUZZLE_SIZE * PUZZLE_SIZE];
    int empty_pos;
    PuzzleState state;
    int moves;
    Uint32 start_time;
    Uint32 remaining_time; // en millisecondes
    TTF_Font* font;
    SDL_Color text_color;
    int hover_piece;
    SDL_Surface* timer_sheet;
    SDL_Rect timer_frames[12];
    int current_timer_frame;
} PuzzleGame;

// Fonctions du puzzle
bool is_valid_move(PuzzleGame* game, int piece_index);
void puzzle_init(PuzzleGame* game, const char* image_paths[]);
void puzzle_handle_event(PuzzleGame* game, SDL_Event* event);
void puzzle_update(PuzzleGame* game);
void puzzle_render(PuzzleGame* game, SDL_Surface* screen);
void puzzle_cleanup(PuzzleGame* game);
bool puzzle_check_win_condition(PuzzleGame* game); // *** Prototype Ajouté/Confirmé ***

#endif // PUZZLE_H
