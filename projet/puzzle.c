#include "puzzle.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

bool puzzle_check_win_condition(PuzzleGame* game); // Prototype ajouté pour clarté


void puzzle_init(PuzzleGame* game, const char* image_paths[]) {
    printf(" Initializing Puzzle...\n");
    game->state = PUZZLE_STATE_RUNNING; game->moves = 0; game->empty_pos = PUZZLE_SIZE * PUZZLE_SIZE - 1; game->remaining_time = TIMER_DURATION * 1000; game->start_time = SDL_GetTicks(); game->font = NULL; game->text_color = (SDL_Color){0, 0, 0, 255}; game->hover_piece = -1; game->selected_piece = -1; game->need_redraw = true; game->background = NULL; game->background_music = NULL; game->timer_sheet = NULL;
    for(int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) { game->piece_locked[i] = false; game->pieces[i] = NULL; }
    game->background = IMG_Load("background.jpg"); if (!game->background) { printf("  WARN: Failed to load puzzle background 'background.jpg': %s\n", IMG_GetError()); }
    game->background_music = Mix_LoadMUS("musique.mp3"); if(!game->background_music) { printf("  WARN: Failed to load puzzle music 'musique.mp3': %s\n", Mix_GetError()); }
    if(game->background_music) { Mix_PlayMusic(game->background_music, -1); }
    int piece_width = 0, piece_height = 0; bool load_error = false; printf("  Loading puzzle pieces...\n");
    for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) { game->pieces[i] = IMG_Load(image_paths[i]); if (!game->pieces[i]) { printf("    ERROR loading piece '%s': %s\n", image_paths[i], IMG_GetError()); load_error = true; } else if (i == 0) { piece_width = game->pieces[i]->w; piece_height = game->pieces[i]->h; } if (piece_width > 0 && piece_height > 0) { game->correct_positions[i].x = (i % PUZZLE_SIZE) * piece_width + 100; game->correct_positions[i].y = (i / PUZZLE_SIZE) * piece_height + 160; game->correct_positions[i].w = piece_width; game->correct_positions[i].h = piece_height; game->positions[i] = game->correct_positions[i]; } else if (i == 0 && load_error) { printf("    FATAL: Cannot determine piece size because piece 0 failed to load.\n"); } }
    if (load_error || piece_width == 0) { printf("  ERROR: Puzzle init failed due to missing piece images.\n"); game->state = PUZZLE_STATE_LOST; /* Marquer comme échoué */ return; } // Modifier pour ne pas quitter directement
    printf("  Shuffling puzzle...\n"); srand(time(NULL));
    for (int shuffle_count = 0; shuffle_count < 1000; shuffle_count++) { int possible_moves[4]; int num_possible = 0; int empty_r = game->empty_pos / PUZZLE_SIZE; int empty_c = game->empty_pos % PUZZLE_SIZE; if (empty_r > 0) possible_moves[num_possible++] = game->empty_pos - PUZZLE_SIZE; if (empty_r < PUZZLE_SIZE - 1) possible_moves[num_possible++] = game->empty_pos + PUZZLE_SIZE; if (empty_c > 0) possible_moves[num_possible++] = game->empty_pos - 1; if (empty_c < PUZZLE_SIZE - 1) possible_moves[num_possible++] = game->empty_pos + 1; if (num_possible > 0) { int move_choice = possible_moves[rand() % num_possible]; SDL_Rect temp = game->positions[move_choice]; game->positions[move_choice] = game->positions[game->empty_pos]; game->positions[game->empty_pos] = temp; game->empty_pos = move_choice; } }
    game->positions[game->empty_pos].x = game->correct_positions[PUZZLE_SIZE*PUZZLE_SIZE-1].x; game->positions[game->empty_pos].y = game->correct_positions[PUZZLE_SIZE*PUZZLE_SIZE-1].y;
    printf("  Loading timer sheet...\n"); game->timer_sheet = IMG_Load("chronometre.png");
    if (!game->timer_sheet) { printf("  WARN: Failed to load timer sheet 'chronometre.png': %s\n", IMG_GetError()); }
    else { int frame_width = 817 / 6; int frame_height = 305 / 2; for (int i = 0; i < 12; i++) { int col = i % 6; int row = i / 6; game->timer_frames[i] = (SDL_Rect){col * frame_width, row * frame_height, frame_width, frame_height}; } }
    game->current_timer_frame = 0; printf(" Puzzle Initialized.\n");
}

bool is_valid_move(PuzzleGame* game, int piece_index) { int empty_row = game->empty_pos / PUZZLE_SIZE; int empty_col = game->empty_pos % PUZZLE_SIZE; int piece_row = piece_index / PUZZLE_SIZE; int piece_col = piece_index % PUZZLE_SIZE; return ((abs(empty_row - piece_row) == 1 && empty_col == piece_col) || (abs(empty_col - piece_col) == 1 && empty_row == piece_row)); }

bool puzzle_check_win_condition(PuzzleGame* game) { for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) { if (i == game->empty_pos) continue; if (game->positions[i].x != game->correct_positions[i].x || game->positions[i].y != game->correct_positions[i].y) { return false; } } return true; }

void puzzle_handle_event(PuzzleGame* game, SDL_Event* event) { if (game->state != PUZZLE_STATE_RUNNING) return; int mouse_x, mouse_y; SDL_GetMouseState(&mouse_x, &mouse_y); game->hover_piece = -1; for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) { if (i != game->empty_pos && !game->piece_locked[i] && mouse_x >= game->positions[i].x && mouse_x < game->positions[i].x + game->positions[i].w && mouse_y >= game->positions[i].y && mouse_y < game->positions[i].y + game->positions[i].h) { game->hover_piece = i; break; } } if (event->type == SDL_MOUSEBUTTONDOWN) { if (event->button.button == SDL_BUTTON_LEFT && game->hover_piece != -1) { game->selected_piece = game->hover_piece; game->drag_offset.x = mouse_x - game->positions[game->selected_piece].x; game->drag_offset.y = mouse_y - game->positions[game->selected_piece].y; game->need_redraw = true; } } else if (event->type == SDL_MOUSEMOTION) { if (game->selected_piece != -1) { game->positions[game->selected_piece].x = mouse_x - game->drag_offset.x; game->positions[game->selected_piece].y = mouse_y - game->drag_offset.y; game->need_redraw = true; } } else if (event->type == SDL_MOUSEBUTTONUP) { if (event->button.button == SDL_BUTTON_LEFT && game->selected_piece != -1) { int released_piece_index = game->selected_piece; game->selected_piece = -1; int target_slot = -1; int piece_center_x = game->positions[released_piece_index].x + game->positions[released_piece_index].w / 2; int piece_center_y = game->positions[released_piece_index].y + game->positions[released_piece_index].h / 2; for (int slot = 0; slot < PUZZLE_SIZE * PUZZLE_SIZE; ++slot) { if (slot == game->empty_pos) { int slot_center_x = game->correct_positions[slot].x + game->correct_positions[slot].w / 2; int slot_center_y = game->correct_positions[slot].y + game->correct_positions[slot].h / 2; int dx = piece_center_x - slot_center_x; int dy = piece_center_y - slot_center_y; int dist_sq = dx * dx + dy * dy; int threshold = (game->correct_positions[slot].w * game->correct_positions[slot].w) / 4; if (dist_sq < threshold) { target_slot = slot; break; } } } if (target_slot == game->empty_pos && is_valid_move(game, released_piece_index)) { game->positions[game->empty_pos] = game->positions[released_piece_index]; game->positions[released_piece_index] = game->correct_positions[game->empty_pos]; game->empty_pos = released_piece_index; game->moves++; for (int j = 0; j < PUZZLE_SIZE * PUZZLE_SIZE; ++j) { if (j != game->empty_pos) { bool is_correct = (game->positions[j].x == game->correct_positions[j].x && game->positions[j].y == game->correct_positions[j].y); if (is_correct && !game->piece_locked[j]) { printf("  Piece %d locked!\n", j); } game->piece_locked[j] = is_correct; } else { game->piece_locked[j] = false; } } if (puzzle_check_win_condition(game)) { game->state = PUZZLE_STATE_WON; if (game->background_music) Mix_HaltMusic(); } } else { int original_slot = -1; for(int slot=0; slot < PUZZLE_SIZE*PUZZLE_SIZE; ++slot) { if(slot == released_piece_index) { original_slot = slot; break; } } if(original_slot != -1) { game->positions[released_piece_index] = game->correct_positions[original_slot]; } } game->need_redraw = true; } } }

void puzzle_update(PuzzleGame* game) { if (game->state != PUZZLE_STATE_RUNNING) return; Uint32 current_time = SDL_GetTicks(); Uint32 elapsed_since_last_update = current_time - game->start_time; if (game->remaining_time > elapsed_since_last_update) { game->remaining_time -= elapsed_since_last_update; } else { game->remaining_time = 0; game->state = PUZZLE_STATE_LOST; if (game->background_music) Mix_HaltMusic(); } game->start_time = current_time; if(game->timer_sheet) { int seconds_total_elapsed = (TIMER_DURATION * 1000 - game->remaining_time) / 1000; game->current_timer_frame = seconds_total_elapsed / 5; if (game->current_timer_frame < 0) game->current_timer_frame = 0; if (game->current_timer_frame > 11) game->current_timer_frame = 11; } }

void puzzle_render(PuzzleGame* game, SDL_Surface* screen) {
    if (!screen) return;
    if (game->background) { SDL_Rect bgDest = {0, 0, screen->w, screen->h}; SDL_BlitSurface(game->background, NULL, screen, &bgDest); }
    else { SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 180, 180, 220)); }

    for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) {
        if (i != game->empty_pos && i != game->selected_piece && game->pieces[i]) {
             if (i == game->hover_piece && !game->piece_locked[i]) { SDL_Rect highlight = game->positions[i]; highlight.x -= 2; highlight.y -= 2; highlight.w += 4; highlight.h += 4; SDL_FillRect(screen, &highlight, SDL_MapRGB(screen->format, 255, 255, 0)); }
             SDL_BlitSurface(game->pieces[i], NULL, screen, &game->positions[i]);
             if (game->piece_locked[i]) { SDL_Surface* overlay = SDL_CreateRGBSurface(SDL_SWSURFACE, game->positions[i].w, game->positions[i].h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000); if(overlay) { SDL_FillRect(overlay, NULL, SDL_MapRGB(screen->format, 150, 150, 150)); SDL_SetAlpha(overlay, SDL_SRCALPHA, 128); SDL_BlitSurface(overlay, NULL, screen, &game->positions[i]); SDL_FreeSurface(overlay); } }
        }
    }
    if (game->selected_piece != -1 && game->pieces[game->selected_piece]) { SDL_BlitSurface(game->pieces[game->selected_piece], NULL, screen, &game->positions[game->selected_piece]); }

    if (game->font) {
        // *** Utilise TOUJOURS la couleur de base pour le texte du timer ***
        SDL_Color textColor = game->text_color;
        /* // SUPPRIMÉ: Changement de couleur du texte
        if (game->state == PUZZLE_STATE_RUNNING && game->remaining_time < 10000) {
             if ((SDL_GetTicks() / 200) % 2) {
                  textColor = (SDL_Color){255, 0, 0, 255};
             }
        }
        */

        int seconds = (game->remaining_time + 999) / 1000; char timer_str[20]; snprintf(timer_str, sizeof(timer_str), "Time: %02d:%02d", seconds / 60, seconds % 60);
        SDL_Surface* timer_surface = TTF_RenderText_Solid(game->font, timer_str, textColor); // Utilise toujours textColor (noir par défaut)
        if (timer_surface) { SDL_Rect timer_pos = {10, 10, 0, 0}; SDL_BlitSurface(timer_surface, NULL, screen, &timer_pos); SDL_FreeSurface(timer_surface); }
        char moves_str[20]; snprintf(moves_str, sizeof(moves_str), "Moves: %d", game->moves);
        SDL_Surface* moves_surface = TTF_RenderText_Solid(game->font, moves_str, game->text_color); if(moves_surface) { SDL_Rect moves_pos = {10, 50, 0, 0}; SDL_BlitSurface(moves_surface, NULL, screen, &moves_pos); SDL_FreeSurface(moves_surface); }
    }

    if (game->timer_sheet) {
        SDL_Rect timer_sprite_dest_pos; int chrono_display_width = 100; int chrono_display_height = 100; timer_sprite_dest_pos.x = (screen->w - chrono_display_width) / 2; timer_sprite_dest_pos.y = 10; timer_sprite_dest_pos.w = chrono_display_width; timer_sprite_dest_pos.h = chrono_display_height;
        int frame_index = game->current_timer_frame; if(frame_index < 0) frame_index = 0; if(frame_index > 11) frame_index = 11;
        SDL_BlitSurface(game->timer_sheet, &game->timer_frames[frame_index], screen, &timer_sprite_dest_pos);
        // *** Clignotement de la BORDURE ROUGE autour du sprite (INCHANGÉ) ***
        if (game->state == PUZZLE_STATE_RUNNING && game->remaining_time < 10000 && (SDL_GetTicks() / 200) % 2) {
            SDL_Rect warn_rect = timer_sprite_dest_pos; warn_rect.x -= 5; warn_rect.y -= 5; warn_rect.w += 10; warn_rect.h += 10;
            SDL_Surface* warn_overlay = SDL_CreateRGBSurface(SDL_SWSURFACE, warn_rect.w, warn_rect.h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
            if(warn_overlay){ SDL_FillRect(warn_overlay, NULL, SDL_MapRGB(screen->format, 255, 0, 0)); SDL_SetAlpha(warn_overlay, SDL_SRCALPHA, 100); SDL_BlitSurface(warn_overlay, NULL, screen, &warn_rect); SDL_FreeSurface(warn_overlay); }
        }
    }

    if (game->state == PUZZLE_STATE_WON || game->state == PUZZLE_STATE_LOST) { SDL_Surface* msg_surface = NULL; const char* message = (game->state == PUZZLE_STATE_WON) ? "You Won!" : "Time's Up!"; SDL_Color msg_color = (game->state == PUZZLE_STATE_WON) ? (SDL_Color){0, 200, 0, 255} : (SDL_Color){200, 0, 0, 255}; if(game->font) { TTF_Font* large_font = TTF_OpenFont("arial.ttf", 48); if (!large_font) large_font = game->font; msg_surface = TTF_RenderText_Solid(large_font, message, msg_color); if (large_font != game->font) TTF_CloseFont(large_font); } if (msg_surface) { SDL_Rect msg_pos; msg_pos.x = (screen->w - msg_surface->w) / 2; msg_pos.y = (screen->h - msg_surface->h) / 2; msg_pos.w = msg_surface->w; msg_pos.h = msg_surface->h; SDL_Rect bg_msg_rect = msg_pos; bg_msg_rect.x -= 20; bg_msg_rect.y -= 10; bg_msg_rect.w += 40; bg_msg_rect.h += 20; SDL_Surface* msg_bg = SDL_CreateRGBSurface(SDL_SWSURFACE, bg_msg_rect.w, bg_msg_rect.h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000); if(msg_bg){ SDL_FillRect(msg_bg, NULL, SDL_MapRGB(screen->format, 50, 50, 50)); SDL_SetAlpha(msg_bg, SDL_SRCALPHA, 180); SDL_BlitSurface(msg_bg, NULL, screen, &bg_msg_rect); SDL_FreeSurface(msg_bg); } SDL_BlitSurface(msg_surface, NULL, screen, &msg_pos); SDL_FreeSurface(msg_surface); } }
}


void puzzle_cleanup(PuzzleGame* game) {
    printf(" Cleaning up Puzzle...\n");
    for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) { if (game->pieces[i]) { SDL_FreeSurface(game->pieces[i]); game->pieces[i] = NULL; } }
    if (game->background) { SDL_FreeSurface(game->background); game->background = NULL; }
    if (game->timer_sheet) { SDL_FreeSurface(game->timer_sheet); game->timer_sheet = NULL; }
    if(game->background_music) { Mix_HaltMusic(); Mix_FreeMusic(game->background_music); game->background_music = NULL; }
     printf(" Puzzle Cleanup Done.\n");
}
