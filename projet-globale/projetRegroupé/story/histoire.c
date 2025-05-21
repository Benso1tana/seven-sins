// Version SDL 1.2 avec fond centré, texte blanc, retour à la ligne et bouton Skip + redimension dynamique
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define INIT_WINDOW_WIDTH 1024
#define INIT_WINDOW_HEIGHT 768
#define MAX_PARTICLES 300
#define TEXT_SPEED 40

int WINDOW_WIDTH = INIT_WINDOW_WIDTH;
int WINDOW_HEIGHT = INIT_WINDOW_HEIGHT;

typedef struct {
    float x, y, dx, dy;
    int life;
    SDL_Color color;
} Particle;

typedef struct {
    Particle particles[MAX_PARTICLES];
    int lastParticleTime;
} ParticleSystem;

typedef struct {
    const char* text;
} Dialogue;

Dialogue dialogues[] = {
    {"Kailos, un jeune aventurier au regard aussi vif que l'eclair, se reveilla dans une foret inconnue, l'esprit vide. Sa memoire avait disparu, effacee comme une fresque sous la pluie. Seul un nom lui revenait en echo : Kailos."},
    {"Pour retrouver son passe, une voix mysterieuse lui murmura qu'il devait affronter les Sept Peches, ces ombres qui rongeaient son ame. Chaque epreuve lui arracherait un fragment de verite, mais a quel prix ?"},
    {"Au terme de ce voyage, l'ultime bataille l'attendait : un face-a-face avec la Source de ses Cauchemars, ce monstre tapi au plus profond de son etre."},
    {"Et quand viendrait l'aube, apres la derniere blessure et le dernier sanglot, Kailos pourrait enfin se reveiller, libere de ses chaines, entier a nouveau."},
    {"Mais son destin ne tenait qu'a un fil...Entre tes mains, joueur, repose l'avenir de Kailos."}
};

int currentDialogue = 0;
char displayBuffer[2048] = "";
int charIndex = 0;
Uint32 lastCharTime = 0;

void InitParticles(ParticleSystem* system) {
    srand(time(NULL));
    for (int i = 0; i < MAX_PARTICLES; i++) system->particles[i].life = 0;
    system->lastParticleTime = 0;
}

void AddParticle(ParticleSystem* system, int x, int y, SDL_Color color) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (system->particles[i].life <= 0) {
            Particle* p = &system->particles[i];
            p->x = x; p->y = y;
            p->dx = (rand() % 100 - 50) / 50.0f;
            p->dy = (rand() % 100 - 50) / 50.0f;
            p->life = rand() % 50 + 50;
            p->color = color;
            break;
        }
    }
}

void UpdateParticles(ParticleSystem* system) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (system->particles[i].life > 0) {
            system->particles[i].x += system->particles[i].dx;
            system->particles[i].y += system->particles[i].dy;
            system->particles[i].dy += 0.05f;
            system->particles[i].life--;
        }
    }
}

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h) return;
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y * surface->w + x] = pixel;
}

void RenderParticles(SDL_Surface* screen, ParticleSystem* system) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (system->particles[i].life > 0) {
            SDL_Color c = system->particles[i].color;
            Uint32 color = SDL_MapRGB(screen->format, c.r, c.g, c.b);
            PutPixel(screen, (int)system->particles[i].x, (int)system->particles[i].y, color);
        }
    }
}

void afficher_histoire(SDL_Surface* screen) {
    // Assume SDL, TTF, IMG are already initialized and screen is valid
    int local_WINDOW_WIDTH = screen->w;
    int local_WINDOW_HEIGHT = screen->h;
    
    SDL_Surface* fuji = IMG_Load("story/fuji.png");
    if (!fuji) fprintf(stderr, "Erreur chargement image: %s\n", IMG_GetError());

    TTF_Font* font = TTF_OpenFont("DejaVuSans-Bold.ttf", 18);
    if (!font) font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 18);
    if (!font) font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 18);
    if (!font) {
        fprintf(stderr, "Erreur chargement police: %s\n", TTF_GetError());
        if (fuji) SDL_FreeSurface(fuji);
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color buttonColor = {30, 30, 30, 255};
    SDL_Color white = {255, 255, 255, 255};

    SDL_Rect button = {local_WINDOW_WIDTH - 160, local_WINDOW_HEIGHT - 70, 120, 40};
    ParticleSystem particles;
    InitParticles(&particles);

    currentDialogue = 0;
    memset(displayBuffer, 0, sizeof(displayBuffer));
    charIndex = 0;
    lastCharTime = 0;

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                running = false;
            else if (e.type == SDL_VIDEORESIZE) {
                local_WINDOW_WIDTH = e.resize.w;
                local_WINDOW_HEIGHT = e.resize.h;
                button.x = local_WINDOW_WIDTH - 160;
                button.y = local_WINDOW_HEIGHT - 70;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if (x >= button.x && x <= button.x + button.w && y >= button.y && y <= button.y + button.h) {
                    currentDialogue++;
                    if (currentDialogue >= sizeof(dialogues) / sizeof(dialogues[0])) {
                        running = false;
                    } else {
                        memset(displayBuffer, 0, sizeof(displayBuffer));
                        charIndex = 0;
                    }
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if (charIndex < strlen(dialogues[currentDialogue].text) && now - lastCharTime > TEXT_SPEED && font && fuji) {
            displayBuffer[charIndex++] = dialogues[currentDialogue].text[charIndex];
            lastCharTime = now;
        }

        if (now - particles.lastParticleTime > 50) {
            SDL_Color pc = {180, 180, 255};
            AddParticle(&particles, rand() % local_WINDOW_WIDTH, rand() % local_WINDOW_HEIGHT, pc);
            particles.lastParticleTime = now;
        }

        UpdateParticles(&particles);

        SDL_FillRect(screen, NULL, 0);

        if (fuji) {
            SDL_Rect centered = {
                (local_WINDOW_WIDTH - fuji->w) / 2,
                (local_WINDOW_HEIGHT - fuji->h) / 2,
                fuji->w,
                fuji->h
            };
            SDL_BlitSurface(fuji, NULL, screen, &centered);
        }

        RenderParticles(screen, &particles);

        if (font && displayBuffer[0]) {
            int y = 50;
            char buffer[2048];
            strncpy(buffer, displayBuffer, sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';

            char* token = strtok(buffer, " ");
            char line[512] = "";
            while (token) {
                char temp[512];
                snprintf(temp, sizeof(temp) - 1, "%s %s", line, token);
                temp[sizeof(temp) - 1] = '\0';
                int w = 0, h = 0;
                TTF_SizeUTF8(font, temp, &w, &h);
                if (w > local_WINDOW_WIDTH - 60) {
                    SDL_Surface* s = TTF_RenderUTF8_Blended(font, line, textColor);
                    if (s) {
                        SDL_Rect r = {30, y};
                        SDL_BlitSurface(s, NULL, screen, &r);
                        SDL_FreeSurface(s);
                        y += h + 5;
                    }
                    strncpy(line, token, sizeof(line));
                    line[sizeof(line) - 1] = '\0';
                } else {
                    strncpy(line, temp, sizeof(line));
                    line[sizeof(line) - 1] = '\0';
                }
                token = strtok(NULL, " ");
            }
            if (strlen(line)) {
                SDL_Surface* s = TTF_RenderUTF8_Blended(font, line, textColor);
                if (s) {
                    SDL_Rect r = {30, y};
                    SDL_BlitSurface(s, NULL, screen, &r);
                    SDL_FreeSurface(s);
                }
            }
        }

        SDL_FillRect(screen, &button, SDL_MapRGB(screen->format, buttonColor.r, buttonColor.g, buttonColor.b));
        SDL_Surface* skipText = TTF_RenderUTF8_Blended(font, "Next", white);
        if (skipText) {
            SDL_Rect r = {button.x + (button.w - skipText->w) / 2, button.y + (button.h - skipText->h) / 2};
            SDL_BlitSurface(skipText, NULL, screen, &r);
            SDL_FreeSurface(skipText);
        }

        SDL_Flip(screen);
        SDL_Delay(16);
    }

    if (fuji) SDL_FreeSurface(fuji);
    if (font) TTF_CloseFont(font);
}
