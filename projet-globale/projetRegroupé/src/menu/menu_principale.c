#include "menu_principale.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>


void init_menu(Menu *M)
{

M -> Bg_principale = IMG_Load("assets/images/ui/Bg_principale.png");
if (M->Bg_principale == NULL)
    {
        printf("Erreur chargement image : %s\n", IMG_GetError());
}
// Music_fond
M->bg_music=Mix_LoadMUS("assets/sounds/SB.mp3");
Mix_PlayMusic(M->bg_music,-1);






M->jouer1[0] = IMG_Load("assets/images/ui/jouer1.png");
M->jouer1[1] = IMG_Load("assets/images/ui/jouer2.png");

M->option1[0]= IMG_Load("assets/images/ui/option1.png");
M->option1[1]=IMG_Load("assets/images/ui/option2.png");

M->meilleurs1[0]=IMG_Load("assets/images/ui/meilleurs1.png");
M->meilleurs1[1]=IMG_Load("assets/images/ui/meilleurs2.png");

M->historique1[0]=IMG_Load("assets/images/ui/historique1.png");
M->historique1[1]=IMG_Load("assets/images/ui/historique2.png");

M->quitter1[0]=IMG_Load("assets/images/ui/quitter1.png");
M->quitter1[1]=IMG_Load("assets/images/ui/quitter2.png");

// Add stage2 button images
M->stage2[0]=IMG_Load("assets/images/ui/jouer1.png");  // Use jouer1.png temporarily
if (!M->stage2[0]) {
    printf("Failed to load stage2_1.png: %s\n", IMG_GetError());
    M->stage2[0] = NULL;
}
M->stage2[1]=IMG_Load("assets/images/ui/jouer2.png");  // Use jouer2.png temporarily
if (!M->stage2[1]) {
    printf("Failed to load stage2_2.png: %s\n", IMG_GetError());
    M->stage2[1] = NULL;
}

// Only set position if images loaded successfully
if (M->stage2[0]) {
    M->pos_stage2.x=775;
    M->pos_stage2.y=175;
    M->pos_stage2.w=M->stage2[0]->w;
    M->pos_stage2.h=M->stage2[0]->h;
} else {
    // Set default position if image failed to load
    M->pos_stage2.x=0;
    M->pos_stage2.y=0;
    M->pos_stage2.w=0;
    M->pos_stage2.h=0;
}

M->pos_jouer1.x= 330;
M->pos_jouer1.y=175;
M->pos_jouer1.w=M->jouer1[0]->w;
M->pos_jouer1.h=M->jouer1[0]->h;

M->pos_option1.x= 330;
M->pos_option1.y=285;
M->pos_option1.w=M->option1[0]->w;
M->pos_option1.h=M->option1[0]->h;

M->pos_meilleurs1.x=330;
M->pos_meilleurs1.y=393;
M->pos_meilleurs1.w=M->meilleurs1 [0]->w;
M->pos_meilleurs1.h=M->meilleurs1[0]->h;

M->pos_historique1.x= 330;
M->pos_historique1.y=500;
M->pos_historique1.w=M->historique1[0]->w;
M->pos_historique1.h=M->historique1[0]->h;

M->pos_quitter1.x=775;
M->pos_quitter1.y=500;
M->pos_quitter1.w=M->quitter1[0]->w;
M->pos_quitter1.h=M->quitter1 [0]->h;

// sous_option

M->bg_option=IMG_Load("assets/images/ui/bg4.png");

M->option_augmenter[0]=IMG_Load("assets/images/ui/augmenter1.png");
M->option_augmenter[1]=IMG_Load("assets/images/ui/augmenter2.png");

M->option_diminuer[0]=IMG_Load("assets/images/ui/diminuer1.png");
M->option_diminuer[1]=IMG_Load("assets/images/ui/diminuer2.png");

M->option_normale[0]=IMG_Load("assets/images/ui/normal1.png");
M->option_normale[1]=IMG_Load("assets/images/ui/normal2.png");

M->option_plein[0]=IMG_Load("assets/images/ui/plein1.png");
M->option_plein[1]=IMG_Load("assets/images/ui/plein2.png");
M->retour[0]=IMG_Load("assets/images/ui/retour1.png");
M->retour[1]=IMG_Load("assets/images/ui/retour2.png");

M->pos_augmenter.x= 360;
M->pos_augmenter.y= 240;
M->pos_augmenter.w=M->option_augmenter[0]->w;
M->pos_augmenter.h=M->option_augmenter[0]->h;

M->pos_diminuer.x= 680;
M->pos_diminuer.y=240;
M->pos_diminuer.w=M->option_diminuer[0]->w;
M->pos_diminuer.h=M->option_diminuer[0]->h;

M->pos_normale.x=360;
M->pos_normale.y=460;
M->pos_normale.w=M->option_normale[0]->w;
M->pos_normale.h=M->option_normale[0]->h;

M->pos_plein.x= 700;
M->pos_plein.y= 460;
M->pos_plein.w=M->option_plein[0]->w;
M->pos_plein.h=M->option_plein[0]->h;

M->pos_retour.x=540;
M->pos_retour.y=570;
M->pos_retour.w=M->retour[0]->w;
M->pos_retour.h=M->retour[0]->h;

M->volume_music=50;
Mix_VolumeMusic(M->volume_music);
M->etat_plein_ecran=0;

//sous menu charger et sauveg
M->bg_sauvegarde_charg[0]=IMG_Load("assets/images/ui/bg3.png");
M->bg_sauvegarde_charg[1]=IMG_Load("assets/images/ui/bg6.png");//s_s chargement

M->bouton_oui[0]=IMG_Load("assets/images/ui/oui1.png");
M->bouton_oui[1]=IMG_Load("assets/images/ui/oui2.png");

M->bouton_non[0]=IMG_Load("assets/images/ui/non1.png");
M->bouton_non[1]=IMG_Load("assets/images/ui/non2.png");

M->bouton_charger[0]=IMG_Load("assets/images/ui/charger1.png");
M->bouton_charger[1]=IMG_Load("assets/images/ui/charger2.png");

M->bouton_nouvelle[0]=IMG_Load("assets/images/ui/nouvelle1.png");
M->bouton_nouvelle[1]=IMG_Load("assets/images/ui/nouvelle2.png");

M->pos_bouton_oui.x= 360;
M->pos_bouton_oui.y= 460;
M->pos_bouton_oui.w=M->bouton_oui[0]->w;
M->pos_bouton_oui.h=M->bouton_oui[0]->h;

M->pos_bouton_non.x= 700;
M->pos_bouton_non.y=460;
M->pos_bouton_non.w=M->bouton_non[0]->w;
M->pos_bouton_non.h=M->bouton_non[0]->h;

M->pos_bouton_charger.x= 500;
M->pos_bouton_charger.y= 200;
M->pos_bouton_charger.w=M->bouton_charger[0]->w;
M->pos_bouton_charger.h=M->bouton_charger[0]->h;

M->pos_bouton_nouvelle.x= 500;
M->pos_bouton_nouvelle.y=460;
M->pos_bouton_nouvelle.w=M->bouton_nouvelle[0]->w;
M->pos_bouton_nouvelle.h=M->bouton_nouvelle[0]->h;

M->p_sauveg=0;


//sous menu_joueur

M->bg_M_joueur[0]=IMG_Load("assets/images/ui/bg7.png");
M->bg_M_joueur[1]=IMG_Load("assets/images/ui/bg5.png");

M->bouton_mono[0]=IMG_Load("assets/images/ui/mono1.png");
M->bouton_mono[1]=IMG_Load("assets/images/ui/mono2.png");

M->bouton_multi[0]=IMG_Load("assets/images/ui/multi1.png");
M->bouton_multi[1]=IMG_Load("assets/images/ui/multi2.png");


M->pos_bouton_mono.x= 500;
M->pos_bouton_mono.y= 200;
M->pos_bouton_mono.w=M->bouton_mono[0]->w;
M->pos_bouton_mono.h=M->bouton_mono[0]->h;

M->pos_bouton_multi.x= 500;
M->pos_bouton_multi.y=460;
M->pos_bouton_multi.w=M->bouton_multi[0]->w;
M->pos_bouton_multi.h=M->bouton_multi[0]->h;

M->etat_joueur=0;

M->bouton_j1[0]=IMG_Load("assets/images/ui/haclia1.png");
M->bouton_j1[1]=IMG_Load("assets/images/ui/haclia2.png");

M->bouton_j2[0]=IMG_Load("assets/images/ui/haclios1.png");
M->bouton_j2[1]=IMG_Load("assets/images/ui/haclios2.png");

M->bouton_valider[0]=IMG_Load("assets/images/ui/valider1.png");
M->bouton_valider[1]=IMG_Load("assets/images/ui/valider2.png");

M->bouton_retour_1[0]=IMG_Load("assets/images/ui/retour1.png");
M->bouton_retour_1[1]=IMG_Load("assets/images/ui/retour2.png");

M->pos_bouton_j1.x= 700;
M->pos_bouton_j1.y= 160;
M->pos_bouton_j1.w=M->bouton_j1[0]->w;
M->pos_bouton_j1.h=M->bouton_j1[0]->h;

M->pos_bouton_j2.x=350;
M->pos_bouton_j2.y=160;
M->pos_bouton_j2.w=M->bouton_j2[0]->w;
M->pos_bouton_j2.h=M->bouton_j2[0]->h;

M->pos_bouton_valider.x= 380;
M->pos_bouton_valider.y= 500;
M->pos_bouton_valider.w=M->bouton_valider[0]->w;
M->pos_bouton_valider.h=M->bouton_valider[0]->h;

M->pos_bouton_retour_1.x= 700;
M->pos_bouton_retour_1.y=500;
M->pos_bouton_retour_1.w=M->bouton_retour_1[0]->w;
M->pos_bouton_retour_1.h=M->bouton_retour_1[0]->h;

M->bg_meilleur_s[0]=IMG_Load("assets/images/ui/bgg1.png");
M->bg_meilleur_s[1]=IMG_Load("assets/images/ui/bgg2.png");

M->bouton_valider_1[0]=IMG_Load("assets/images/ui/v1.png");
M->bouton_valider_1[1]=IMG_Load("assets/images/ui/v2.png");

M->pos_bouton_valider_1.x= 520;
M->pos_bouton_valider_1.y= 400;
M->pos_bouton_valider_1.w=M->bouton_valider_1[0]->w;
M->pos_bouton_valider_1.h=M->bouton_valider_1[0]->h;

M->arial=TTF_OpenFont("assets/fonts/arial.ttf",38);
if (M->arial==NULL)
    {
        printf("Erreur chargement image : %s\n", TTF_GetError());
     }

M->text_color.r=0;
M->text_color.b=0;
M->text_color.g=0;

M->pos_saisie.x= 490;
M->pos_saisie.y= 305;

// liste meilleur score

M->bouton_quitter_2[0]=IMG_Load("assets/images/ui/quitter1.png");
M->bouton_quitter_2[1]=IMG_Load("assets/images/ui/quitter2.png");

M->bouton_retour_2[0]=IMG_Load("assets/images/ui/retour1.png");
M->bouton_retour_2[1]=IMG_Load("assets/images/ui/retour2.png");

M->pos_bouton_retour_2.x= 700;
M->pos_bouton_retour_2.y=540;
M->pos_bouton_retour_2.w=M->bouton_retour_2[0]->w;
M->pos_bouton_retour_2.h=M->bouton_retour_2[0]->h;

M->pos_bouton_quitter_2.x= 400;
M->pos_bouton_quitter_2.y=540;
M->pos_bouton_quitter_2.w=M->bouton_quitter_2[0]->w;
M->pos_bouton_quitter_2.h=M->bouton_quitter_2[0]->h;

}

void Afficher_menu(Menu M,SDL_Surface *ecran)
{
    SDL_BlitSurface (M.Bg_principale,NULL,ecran,NULL);

    if(selectionner_bouton(M.pos_jouer1)==0)
    {
        SDL_BlitSurface (M.jouer1[0],NULL,ecran,&M.pos_jouer1);
    }
    else
    {
        SDL_BlitSurface (M.jouer1[1],NULL,ecran,&M.pos_jouer1);
    }

    if(selectionner_bouton(M.pos_option1)==0){
        SDL_BlitSurface (M.option1[0],NULL,ecran,&M.pos_option1);
    }
    else
    {
        SDL_BlitSurface (M.option1[1],NULL,ecran,&M.pos_option1);
    }

    if(selectionner_bouton(M.pos_meilleurs1)==0)
    {
        SDL_BlitSurface (M.meilleurs1[0],NULL,ecran,&M.pos_meilleurs1);
    }
    else
    {
        SDL_BlitSurface (M.meilleurs1[1],NULL,ecran,&M.pos_meilleurs1);
    }

    if(selectionner_bouton(M.pos_historique1)==0)
    {
        SDL_BlitSurface (M.historique1[0],NULL,ecran,&M.pos_historique1);
    }
    else
    {
        SDL_BlitSurface (M.historique1[1],NULL,ecran,&M.pos_historique1);
    }

    if(selectionner_bouton(M.pos_quitter1)==0)
    {
        SDL_BlitSurface (M.quitter1[0],NULL,ecran,&M.pos_quitter1);
    }
    else
    {
        SDL_BlitSurface (M.quitter1[1],NULL,ecran,&M.pos_quitter1);
    }

    // Add stage2 button display
    if (M.stage2[0] && M.stage2[1]) {  // Only display if both images loaded successfully
        if(selectionner_bouton(M.pos_stage2)==0)
        {
            SDL_BlitSurface (M.stage2[0],NULL,ecran,&M.pos_stage2);
        }
        else
        {
            SDL_BlitSurface (M.stage2[1],NULL,ecran,&M.pos_stage2);
        }
    }
}



void Afficher_menu_option(Menu M,SDL_Surface *ecran)
{
    SDL_BlitSurface (M.bg_option,NULL,ecran,NULL);

     if(selectionner_bouton(M.pos_augmenter)==0)
  {
      SDL_BlitSurface (M.option_augmenter[0],NULL,ecran,&M.pos_augmenter);
  }
  else
  {
    SDL_BlitSurface (M.option_augmenter[1],NULL,ecran,&M.pos_augmenter);
  }
if(selectionner_bouton(M.pos_diminuer)==0){

  SDL_BlitSurface (M.option_diminuer[0],NULL,ecran,&M.pos_diminuer);
}
else
{
    SDL_BlitSurface (M.option_diminuer[1],NULL,ecran,&M.pos_diminuer);
}
if(selectionner_bouton(M.pos_normale)==0)
{
  SDL_BlitSurface (M.option_normale[0],NULL,ecran,&M.pos_normale);
}
else
{
    SDL_BlitSurface (M.option_normale[1],NULL,ecran,&M.pos_normale);
}
 if(selectionner_bouton(M.pos_plein)==0)
 {
      SDL_BlitSurface (M.option_plein[0],NULL,ecran,&M.pos_plein);

 }
 else
 {
      SDL_BlitSurface (M.option_plein[1],NULL,ecran,&M.pos_plein);

 }
  if(selectionner_bouton(M.pos_retour)==0)
{
     SDL_BlitSurface (M.retour[0],NULL,ecran,&M.pos_retour);

}
else
{
     SDL_BlitSurface (M.retour[1],NULL,ecran,&M.pos_retour);

}
}


void Afficher_menu_Sauvegarde(Menu M,SDL_Surface *ecran)
{
if(M.p_sauveg==0){

SDL_BlitSurface (M.bg_sauvegarde_charg[0],NULL,ecran,NULL);

if(selectionner_bouton(M.pos_bouton_oui)==0)
  {
      SDL_BlitSurface (M.bouton_oui[0],NULL,ecran,&M.pos_bouton_oui);

  }
  else
  {
    SDL_BlitSurface (M.bouton_oui[1],NULL,ecran,&M.pos_bouton_oui);
  }
}
if(selectionner_bouton(M.pos_bouton_non)==0)
    {

  SDL_BlitSurface (M.bouton_non[0],NULL,ecran,&M.pos_bouton_non);
}
else
{
    SDL_BlitSurface (M.bouton_non[1],NULL,ecran,&M.pos_bouton_non);
}

}
// Affichage sous_menu_joueur


void Afficher_Joueur(Menu M,SDL_Surface *ecran)
{

SDL_BlitSurface (M.bg_M_joueur[0],NULL,ecran,NULL);

if(selectionner_bouton(M.pos_bouton_mono)==0)
  {
      SDL_BlitSurface (M.bouton_mono[0],NULL,ecran,&M.pos_bouton_mono);

  }
  else
  {
    SDL_BlitSurface (M.bouton_mono[1],NULL,ecran,&M.pos_bouton_mono);
  }

if(selectionner_bouton(M.pos_bouton_multi)==0)
    {

  SDL_BlitSurface (M.bouton_multi[0],NULL,ecran,&M.pos_bouton_multi);
}
else
{
    SDL_BlitSurface (M.bouton_multi[1],NULL,ecran,&M.pos_bouton_multi);
}

}





int selectionner_bouton (SDL_Rect pos_bouton)
{
int x_mouse,y_mouse;

SDL_GetMouseState(&x_mouse,&y_mouse);

if((x_mouse>pos_bouton.x) && (x_mouse<(pos_bouton.x+ pos_bouton.w)) &&(y_mouse>pos_bouton.y)&&(y_mouse<(pos_bouton.y+pos_bouton.h)))
{
    return 1;
}
else
{
    return 0;
}
}
void miseajour(Menu *M)
{
    if(selectionner_bouton(M->pos_jouer1)==1)
    {
        M->selection=1;
    }
    else if(selectionner_bouton(M->pos_option1)==1)
    { ;
         M->selection=2;
    }
    else if(selectionner_bouton(M->pos_meilleurs1)==1)
    {
         M->selection=3;

    }
    else if (selectionner_bouton(M->pos_historique1)==1)
    {
        M->selection=4;

    }
    else if(selectionner_bouton(M->pos_quitter1)==1)
        {
           M->selection=5;

        }
        else
        {
            M->selection=0;

        }
}
void Afficher_chargement (Menu M,SDL_Surface *ecran)
{
         SDL_BlitSurface (M.bg_sauvegarde_charg[1],NULL,ecran,NULL);

   if(selectionner_bouton(M.pos_bouton_charger)==0)
    {

       SDL_BlitSurface (M.bouton_charger[0],NULL,ecran,&M.pos_bouton_charger);
    }
     else
    {
    SDL_BlitSurface (M.bouton_charger[1],NULL,ecran,&M.pos_bouton_charger);
     }

if(selectionner_bouton(M.pos_bouton_nouvelle)==0)
    {

      SDL_BlitSurface (M.bouton_nouvelle[0],NULL,ecran,&M.pos_bouton_nouvelle);
    }
else
  {
    SDL_BlitSurface (M.bouton_nouvelle[1],NULL,ecran,&M.pos_bouton_nouvelle);
   }

    }





    void Afficher_Choisir_Joueur(Menu M,SDL_Surface *ecran)
    {
        SDL_BlitSurface (M.bg_M_joueur[1],NULL,ecran,NULL);
          if(selectionner_bouton(M.pos_bouton_j1)==0)
    {

       SDL_BlitSurface (M.bouton_j1[0],NULL,ecran,&M.pos_bouton_j1);
    }
     else
    {
    SDL_BlitSurface (M.bouton_j1[1],NULL,ecran,&M.pos_bouton_j1);
     }

if(selectionner_bouton(M.pos_bouton_j2)==0)
    {

      SDL_BlitSurface (M.bouton_j2[0],NULL,ecran,&M.pos_bouton_j2);
    }
else
  {
    SDL_BlitSurface (M.bouton_j2[1],NULL,ecran,&M.pos_bouton_j2);
   }

 if(selectionner_bouton(M.pos_bouton_valider)==0)
    {

       SDL_BlitSurface (M.bouton_valider[0],NULL,ecran,&M.pos_bouton_valider);
    }
     else
    {
    SDL_BlitSurface (M.bouton_valider[1],NULL,ecran,&M.pos_bouton_valider);
     }

if(selectionner_bouton(M.pos_bouton_retour_1)==0)
    {

      SDL_BlitSurface (M.bouton_retour_1[0],NULL,ecran,&M.pos_bouton_retour_1);
    }
else
  {
    SDL_BlitSurface (M.bouton_retour_1[1],NULL,ecran,&M.pos_bouton_retour_1);
   }


    }




void Afficher_Meilleur_score(Menu M, SDL_Surface *ecran)
{
    char texte_affiche[100];
    int continuer = 1;
    int cursor_visible = 1;
    Uint32 last_cursor_toggle = SDL_GetTicks();

   // SDL_Surface *surface_texte = NULL;
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(200, 50);

    strcpy(M.saisie_texte, "");
    while (continuer)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                exit(0);

                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RETURN)
                    continuer = 0;

                else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(M.saisie_texte) > 0)
                    M.saisie_texte[strlen(M.saisie_texte) - 1] = '\0';

                else if (event.key.keysym.unicode && strlen(M.saisie_texte) < 20)
                {
                    char c = (char)event.key.keysym.unicode;
                    if (c >= 32 && c <= 126)
                    {
                        int len = strlen(M.saisie_texte);
                        M.saisie_texte[len] = c;
                        M.saisie_texte[len + 1] = '\0';
                    }
                }
                break;

            case SDL_MOUSEBUTTONDOWN:

                if(event.button.button==SDL_BUTTON_LEFT)
             {
                    if(selectionner_bouton(M.pos_bouton_valider_1)==1)
                 {
                   /* Afficher_liste(M,ecran);
                     SDL_Flip(ecran);*/
                   continuer=0;
                 }
             }


                break;
        }
    }



        if (SDL_GetTicks() - last_cursor_toggle > 500)
        {
            cursor_visible = !cursor_visible;
            last_cursor_toggle = SDL_GetTicks();
        }


        strcpy(texte_affiche, M.saisie_texte);
        if (cursor_visible)
            strcat(texte_affiche, "|");

        if (M.saisie)
        {
            SDL_FreeSurface(M.saisie);
            M.saisie= NULL;
        }

      M.saisie = TTF_RenderText_Solid(M.arial,texte_affiche,M.text_color);

        // Affichage écran
        SDL_BlitSurface(M.bg_meilleur_s[0], NULL, ecran, NULL);
      SDL_BlitSurface(M.saisie, NULL, ecran, &M.pos_saisie);



         if(selectionner_bouton(M.pos_bouton_valider_1)==0)
    {

       SDL_BlitSurface (M.bouton_valider_1[0],NULL,ecran,&M.pos_bouton_valider_1);
    }
     else
     {
    SDL_BlitSurface (M.bouton_valider_1[1],NULL,ecran,&M.pos_bouton_valider_1);
     }


     SDL_Flip(ecran);
     SDL_Delay(10);


}

      if (M.saisie)
        SDL_FreeSurface(M.saisie);

    SDL_EnableUNICODE(0);
}





void Afficher_liste(Menu M,SDL_Surface *ecran)
{

SDL_BlitSurface(M.bg_meilleur_s[1], NULL, ecran, NULL);


if(selectionner_bouton(M.pos_bouton_quitter_2)==0)
  {
      SDL_BlitSurface (M.bouton_quitter_2[0],NULL,ecran,&M.pos_bouton_quitter_2);

  }
  else
  {
    SDL_BlitSurface (M.bouton_quitter_2[1],NULL,ecran,&M.pos_bouton_quitter_2);
  }

if(selectionner_bouton(M.pos_bouton_retour_2)==0)
    {

  SDL_BlitSurface (M.bouton_retour_2[0],NULL,ecran,&M.pos_bouton_retour_2);
}
else
{
    SDL_BlitSurface (M.bouton_retour_2[1],NULL,ecran,&M.pos_bouton_retour_2);
}

}
