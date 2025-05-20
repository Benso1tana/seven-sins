#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 1675
#define SCREEN_HEIGHT 1081
#define MAX_NAME_LENGTH 20

SDL_Surface *screen = NULL;
SDL_Surface *background3 = NULL, *background4 = NULL;
SDL_Surface *btn_valider = NULL, *btn_valider_hover = NULL;
SDL_Surface *btn_retour = NULL, *btn_retour_hover = NULL;
SDL_Surface *btn_quitter = NULL, *btn_quitter_hover = NULL;
SDL_Surface *texteNom = NULL;
TTF_Font *font = NULL;
SDL_Color textColor = {0, 0, 0};

Mix_Music *musique = NULL;

char nomJoueur[MAX_NAME_LENGTH + 1] = "";
bool entrerNom = true;

typedef enum { MENU_PRINCIPAL, ENIGME } Etat;
Etat etatActuel = MENU_PRINCIPAL;

SDL_Surface* chargerImage(const char* chemin) {
    SDL_Surface* image = IMG_Load(chemin);
    if (!image) {
        printf("Erreur de chargement de l'image %s: %s\n", chemin, SDL_GetError());
        return NULL;
    }
    return image;
}

void afficherBackground(SDL_Surface *background) {
    SDL_BlitSurface(background, NULL, screen, NULL);
}

void afficherTexte(const char *texte) {
    if (texteNom) {
        SDL_FreeSurface(texteNom);
    }
    texteNom = TTF_RenderText_Solid(font, texte, textColor);

    int texteLargeur, texteHauteur;
    TTF_SizeText(font, texte, &texteLargeur, &texteHauteur);

    int posX = (SCREEN_WIDTH - texteLargeur) / 2;
    int posY = 440;

    SDL_FillRect(screen, &(SDL_Rect){posX - 10, posY - 10, texteLargeur + 20, texteHauteur + 20},      SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_Rect pos = {posX, posY};
    SDL_BlitSurface(texteNom, NULL, screen, &pos);
}

bool estSurRectangle(int x, int y, SDL_Rect *rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}

void enregistrerNom() {
    FILE *file = fopen("nom.txt", "w");
    if (file) {
        fprintf(file, "%s\n", nomJoueur);
        fclose(file);
    } else {
        printf("Erreur lors de l'enregistrement du nom.\n");
    }
}

void gererEvenements() {
    SDL_Event event;
    bool hoverValider = false, hoverRetour = false, hoverQuitter = false;

    // Récupérer la taille des images pour définir correctement les rectangles des boutons
    SDL_Rect rectValider = {(SCREEN_WIDTH - btn_valider->w) / 2, 590, btn_valider->w, btn_valider->h};
    SDL_Rect rectRetour = {975, 630, btn_retour->w, btn_retour->h};
    SDL_Rect rectQuitter = {1125, 630, btn_quitter->w, btn_quitter->h};

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                if (entrerNom) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(nomJoueur) > 0) {
                        nomJoueur[strlen(nomJoueur) - 1] = '\0';
                    } else if (event.key.keysym.sym == SDLK_RETURN && strlen(nomJoueur) > 0) {
                        entrerNom = false;
                        etatActuel = ENIGME;
                        enregistrerNom();
                    } else if (strlen(nomJoueur) < MAX_NAME_LENGTH) {
                        char lettre = event.key.keysym.unicode;
                        if (lettre >= 32 && lettre <= 126) {
                            int len = strlen(nomJoueur);
                            nomJoueur[len] = lettre;
                            nomJoueur[len + 1] = '\0';
                        }
                    }
		
                } else {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        exit(0);
                    }
	            if (event.key.keysym.sym == SDLK_r) {  // 🔥 Retour avec touche "R"
                        printf("Retour à l'écran du menu !\n");
                        etatActuel = MENU_PRINCIPAL;
                        entrerNom = true;
                        nomJoueur[0] = '\0';
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                hoverValider = estSurRectangle(event.motion.x, event.motion.y, &rectValider);
                hoverRetour = estSurRectangle(event.motion.x, event.motion.y, &rectRetour);
                hoverQuitter = estSurRectangle(event.motion.x, event.motion.y, &rectQuitter);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (hoverValider && entrerNom) {
                        etatActuel = ENIGME;
                        enregistrerNom();
                    } else if (hoverRetour && !entrerNom) {
                        etatActuel = MENU_PRINCIPAL;
                        entrerNom = true;
                        nomJoueur[0] = '\0';
                    } else if (hoverQuitter) {
                        exit(0);
                    }
                }
                break;
        }
    }

    if (etatActuel == MENU_PRINCIPAL) {
        afficherBackground(background3);
        SDL_BlitSurface(hoverValider ? btn_valider_hover : btn_valider, NULL, screen, &rectValider);
        afficherTexte(nomJoueur);
    } else if (etatActuel == ENIGME) {
        afficherBackground(background4);
        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(musique, -1);
        }
        SDL_BlitSurface(hoverRetour ? btn_retour_hover : btn_retour, NULL, screen, &rectRetour);
        SDL_BlitSurface(hoverQuitter ? btn_quitter_hover : btn_quitter, NULL, screen, &rectQuitter);
    }

    SDL_Flip(screen);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        printf("Erreur lors de l'initialisation de Mix_OpenAudio : %s\n", Mix_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Menu meilleurs scores ", NULL);

    font = TTF_OpenFont("arial.ttf", 28);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

    background3 = chargerImage("menu2.png");
    background4 = chargerImage("menu1.png");
    btn_valider = chargerImage("bttn1.png");
    btn_valider_hover = chargerImage("bttn1.png");
    btn_retour = chargerImage("bnt2.png");
    btn_retour_hover = chargerImage("btn2.png");
    btn_quitter = chargerImage("btn1.png");
    btn_quitter_hover = chargerImage("bnt1.png");

    if (!background3 || !background4 || !btn_valider || !btn_valider_hover || !btn_retour || !btn_retour_hover || !btn_quitter || !btn_quitter_hover) {
        printf("Erreur : Impossible de charger certaines images.\n");
        return 1;
    }

    musique = Mix_LoadMUS("Séquence 01_1.mp3");
    if (!musique) {
        printf("Erreur de chargement de la musique: %s\n", Mix_GetError());
        return 1;
    }

    afficherBackground(background3);
    SDL_EnableUNICODE(1);

    while (1) {
        gererEvenements();
        SDL_Delay(16);
    }

    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}

