#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}


int interprete (sequence_t* seq, bool debug)
{
    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    if (!silent_mode){
      printf ("Programme:");
      afficher(seq);
      printf ("\n");
    }
    if (debug) stop();

    int ret; 

    char commande;

    cellule_t *current_cel;
    cellule_t *debut, *dernier; //pour les chaines de commande
    current_cel = seq->tete;
    pile *pile_calculette = creer_pile();

    while (current_cel != NULL) { 
        printf("%c\n",current_cel->command);
        commande = current_cel->command;
        switch (commande) {
            case 'A':
                ret = avance();
                if (ret == VICTOIRE){
                  detruire_pile_double(pile_calculette);
                  return VICTOIRE;
                }
                if (ret == RATE){
                  detruire_pile_double(pile_calculette);
                  return RATE;
                }   
                break;
            case 'D':
                droite();
                break;
            case 'G':
                gauche();
                break;
            case 'P':
                pose(depiler_int(pile_calculette));
                break;
            case 'M':
                empiler_int(pile_calculette,mesure(depiler_int(pile_calculette)));
                break;/*
            case '+':
                addition(pile_calculette);
                break;
            case '-':
                soustraction(pile_calculette);
                break;
            case '*':
                multiplication(pile_calculette);
                break;*/
            case '?':
                condition(pile_calculette, current_cel);
            case '{':
                debut = current_cel; // attentio deb est peut être NULL
                while(current_cel->suivant->command != '}'){ // on part du principe que le groupe de commande a une fin
                    current_cel = current_cel->suivant;
                }
                dernier = current_cel;
                current_cel = current_cel->suivant;
                dernier->suivant = NULL;
                empiler_groupe_de_commande(pile_calculette, debut->suivant);

            default:
                printf("Caractère spécial: '%c'\n", commande);
                if (commande <= '9' && '0' <= commande){
                  empiler_int(pile_calculette,(commande-'0'));
                }
                afficher_pile_double(pile_calculette);
                break;
        }
        seq->tete = current_cel->suivant;
        free(current_cel);
        current_cel = seq->tete;
        // ce serait bien de free la précédente cel
        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        if (debug) stop();
    }
    
    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    detruire_pile_double(pile_calculette);
    return CIBLERATEE;
}
