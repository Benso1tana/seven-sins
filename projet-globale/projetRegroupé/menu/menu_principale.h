#ifndef MENU_PRINCIPALE_H_INCLUDED
#define MENU_PRINCIPALE_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>



typedef struct
{  //menu principale
    SDL_Surface *Bg_principale;

    SDL_Surface *jouer1 [2];
    SDL_Surface *option1 [2];
    SDL_Surface *meilleurs1[2];
    SDL_Surface *historique1 [2];
    SDL_Surface *quitter1 [2];


    SDL_Rect pos_jouer1;
    SDL_Rect pos_option1 ;
    SDL_Rect pos_meilleurs1;
    SDL_Rect pos_historique1 ;
    SDL_Rect pos_quitter1 ;
     SDL_Rect positionTexte; //zone de saisie
    int selection ; // bouton selectionner
    Mix_Music *bg_music;
    TTF_Font *arial;
    SDL_Color text_color;
    SDL_Surface *saisie;
    SDL_Rect pos_saisie;

  char saisie_texte[20];

    //sous_menu_option

    SDL_Surface *bg_option;
    SDL_Surface *option_augmenter[2];
    SDL_Surface *option_diminuer[2];
    SDL_Surface *option_normale[2];
    SDL_Surface *option_plein[2];
    SDL_Surface *retour[2];

     SDL_Rect pos_augmenter;
    SDL_Rect pos_diminuer ;
    SDL_Rect pos_normale;
    SDL_Rect pos_plein;
    SDL_Rect pos_retour;
  int volume_music;
  int etat_plein_ecran;//plein_ecran



  //sous_menu save_charger
  SDL_Surface *bg_sauvegarde_charg[2];
  SDL_Surface *bouton_oui[2];
  SDL_Surface *bouton_non[2];
  SDL_Surface *bouton_charger[2];
  SDL_Surface *bouton_nouvelle[2];

  SDL_Rect pos_bouton_oui;
  SDL_Rect pos_bouton_non ;
  SDL_Rect pos_bouton_charger;
  SDL_Rect pos_bouton_nouvelle;

   int p_sauveg;

  SDL_Surface *bg_M_joueur[2];
  SDL_Surface *bouton_mono[2];
  SDL_Surface *bouton_multi[2];


  SDL_Rect pos_bouton_mono;
  SDL_Rect pos_bouton_multi;


int etat_joueur;
//choisir_joueur
  SDL_Surface *bouton_j1[2];
  SDL_Surface *bouton_j2[2];

  SDL_Surface *bouton_valider[2];
  SDL_Surface *bouton_retour_1[2];


  SDL_Rect pos_bouton_j1;
  SDL_Rect pos_bouton_j2 ;
  SDL_Rect pos_bouton_valider;
  SDL_Rect pos_bouton_retour_1;


  //sous_menu_meilleur_s


    SDL_Surface *bg_meilleur_s [2];
    SDL_Surface *bouton_valider_1[2];

    SDL_Rect pos_bouton_valider_1;
     // lise_meilleur_score

     SDL_Surface *bouton_quitter_2[2];
     SDL_Surface *bouton_retour_2[2];


     SDL_Rect pos_bouton_retour_2;
     SDL_Rect pos_bouton_quitter_2;





}Menu;

void init_menu(Menu *M);
void Afficher_menu(Menu M,SDL_Surface *ecran);
int selectionner_bouton (SDL_Rect pos_bouton);
void miseajour(Menu *M);


// sous_option

void Afficher_menu_option(Menu M,SDL_Surface *ecran);

//menu sauvegarde
void Afficher_menu_Sauvegarde(Menu M,SDL_Surface *ecran);
void Afficher_chargement(Menu M,SDL_Surface *ecran);
void Afficher_Joueur(Menu M,SDL_Surface *ecran);
void Afficher_Choisir_Joueur(Menu M,SDL_Surface *ecran);
void Afficher_Meilleur_score (Menu M,SDL_Surface *ecran);

void Afficher_liste(Menu M,SDL_Surface *ecran);




#endif
