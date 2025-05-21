#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "menu_principale.c"

#define SCREEN_W 1280
#define SCREEN_H 768

int main (int argc, char *argv[])
{
    SDL_Surface *ecran ;

    // Fenêtre
    ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096);
    SDL_WM_SetCaption("Jeu", NULL);

SDL_Init(SDL_INIT_VIDEO);
if (TTF_Init() == -1) {
    printf("Erreur TTF_Init: %s\n", TTF_GetError());
    return 1;
}


    // Boucle principale
    int Quitter = 0;

    SDL_Event event;

    int indice=0;

    Menu menu;

    init_menu (&menu);
    while(Quitter==0){

    switch (indice){

        //Menu_principale



 case 0:

         Afficher_menu(menu,ecran);
         SDL_Flip(ecran);


     while(SDL_PollEvent(&event))
     {
         switch(event.type)
         {

           case SDL_QUIT:
             Quitter=1;
           break;
           case SDL_MOUSEBUTTONDOWN:
            if(event.button.button==SDL_BUTTON_LEFT)
            {
                if (menu.selection==5)
                {
                    Quitter=1;
                }
                 if (menu.selection==1)
                {
                    // Write selected stage to file
                    FILE *stageFile = fopen("selected_stage.txt", "w");
                    if (stageFile) {
                        fprintf(stageFile, "1");  // Stage 1
                        fclose(stageFile);
                    }
                    indice=3;
                }
                if(menu.selection==3)
                {
                    // Write selected stage to file
                    FILE *stageFile = fopen("selected_stage.txt", "w");
                    if (stageFile) {
                        fprintf(stageFile, "2");  // Stage 2
                        fclose(stageFile);
                    }
                    indice=7;
                }
                if (menu.selection==2)
                {
                    // Just switch to options menu, don't write stage file
                    indice=2;
                }



            }
            break;

         }
     }

        miseajour(&menu);
    break;

    case 1:// menu joueur

 Afficher_Joueur(menu,ecran);
    SDL_Flip(ecran);
    while(SDL_PollEvent(&event))
     {
         switch(event.type)
          {

           case SDL_QUIT:
             Quitter=1;
            break;
          case SDL_MOUSEBUTTONDOWN:
             if(event.button.button==SDL_BUTTON_LEFT)
             {

                 if(selectionner_bouton(menu.pos_bouton_mono)==1)
                 {
                   indice=5;
                 }
                   if(selectionner_bouton(menu.pos_bouton_multi)==1)
                 {
                   indice=5;
                 }
             }
             break;
     }
    }

    break;
           case 2 :

       Afficher_menu_option(menu, ecran);
       SDL_Flip(ecran);


    while(SDL_PollEvent(&event))
     {
         switch(event.type)
         {
           case SDL_QUIT:
             Quitter=1;
           break;
           case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT) {
                // Get mouse position
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                // Check each button
                if(mouseX >= menu.pos_retour.x && mouseX <= menu.pos_retour.x + menu.pos_retour.w &&
                   mouseY >= menu.pos_retour.y && mouseY <= menu.pos_retour.y + menu.pos_retour.h) {
                    indice = 0;  // Return to main menu
                }
                else if(mouseX >= menu.pos_augmenter.x && mouseX <= menu.pos_augmenter.x + menu.pos_augmenter.w &&
                        mouseY >= menu.pos_augmenter.y && mouseY <= menu.pos_augmenter.y + menu.pos_augmenter.h) {
                    menu.volume_music += 10;
                    if(menu.volume_music > 100) menu.volume_music = 100;
                    Mix_VolumeMusic(menu.volume_music);
                }
                else if(mouseX >= menu.pos_diminuer.x && mouseX <= menu.pos_diminuer.x + menu.pos_diminuer.w &&
                        mouseY >= menu.pos_diminuer.y && mouseY <= menu.pos_diminuer.y + menu.pos_diminuer.h) {
                    menu.volume_music -= 10;
                    if(menu.volume_music < 0) menu.volume_music = 0;
                    Mix_VolumeMusic(menu.volume_music);
                }
                else if(mouseX >= menu.pos_plein.x && mouseX <= menu.pos_plein.x + menu.pos_plein.w &&
                        mouseY >= menu.pos_plein.y && mouseY <= menu.pos_plein.y + menu.pos_plein.h) {
                    if(menu.etat_plein_ecran == 0) {
                        ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                        menu.etat_plein_ecran = 1;
                    }
                }
                else if(mouseX >= menu.pos_normale.x && mouseX <= menu.pos_normale.x + menu.pos_normale.w &&
                        mouseY >= menu.pos_normale.y && mouseY <= menu.pos_normale.y + menu.pos_normale.h) {
                    if(menu.etat_plein_ecran == 1) {
                        ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                        menu.etat_plein_ecran = 0;
                    }
                }
            }
            break;
         }
     }
     break;
    case 3 :
    Afficher_menu_Sauvegarde(menu,ecran);
    SDL_Flip(ecran);
    while(SDL_PollEvent(&event))
     {
         switch(event.type)
          {

           case SDL_QUIT:
             Quitter=1;
            break;




         case SDL_MOUSEBUTTONDOWN:
             if(event.button.button==SDL_BUTTON_LEFT)
             {
                 if(selectionner_bouton(menu.pos_bouton_non)==1)
                 {
                   indice=0;
                 }
                 if(selectionner_bouton(menu.pos_bouton_oui)==1)
                 {
                   indice=4;
                 }
             }
             break;
     }
    }
    break;
         case 4 :
             Afficher_chargement(menu,ecran);
             SDL_Flip(ecran);
           while(SDL_PollEvent(&event))
            {
              switch(event.type)
              {

                case SDL_QUIT:
                  Quitter=1;
                  break;
                           case SDL_MOUSEBUTTONDOWN:
             if(event.button.button==SDL_BUTTON_LEFT)
             {
                 if(selectionner_bouton(menu.pos_bouton_nouvelle)==1)
                 {
                   indice=1;
                 }
             }
             break;

              }
            }
         break;
case 5:
Afficher_Choisir_Joueur(menu,ecran);
    SDL_Flip(ecran);
    while(SDL_PollEvent(&event))
     {
         switch(event.type)
          {

           case SDL_QUIT:
             Quitter=1;
            break;
       case SDL_MOUSEBUTTONDOWN:
             if(event.button.button==SDL_BUTTON_LEFT)
             {
                 if(selectionner_bouton(menu.pos_bouton_retour_1)==1)
                 {
                   indice=0;
                 }
                 if(selectionner_bouton(menu.pos_bouton_valider)==1)
                 {
                   // Load stage one
                   system("cd ../stage\\ .1 && ./prog");
                   Quitter = 1;  // Exit menu after launching stage
                 }
             }
             break;
     }
    }

    break;


case 6:
    Afficher_Meilleur_score(menu,ecran);
    SDL_Flip(ecran);
    while(SDL_PollEvent(&event))
     {
         switch(event.type)
          {

           case SDL_QUIT:
             Quitter=1;
            break;
       case SDL_MOUSEBUTTONDOWN:
             if(event.button.button==SDL_BUTTON_LEFT)
             {
                 if(selectionner_bouton(menu.pos_bouton_valider_1)==1)
                 {
                   indice=7;
                 }
             }
             break;
     }
    }

    break;


    case 7:
    Afficher_liste(menu,ecran);
    SDL_Flip(ecran);
    while(SDL_PollEvent(&event))
     {
         switch(event.type)
          {

           case SDL_QUIT:
             Quitter=1;
            break;
       case SDL_MOUSEBUTTONDOWN:
             if(event.button.button==SDL_BUTTON_LEFT)
             {
                 if(selectionner_bouton(menu.pos_bouton_retour_2)==1)
                 {
                   indice=0;
                 }
                 if(selectionner_bouton(menu.pos_bouton_quitter_2)==1)
                 {
                   Quitter=1;
                 }
             }
             break;
     }
    }



       break;






    default :
    break;

    }

    }
    // Libérations

    SDL_FreeSurface(ecran);

    SDL_Quit();
    return 0;

 }
