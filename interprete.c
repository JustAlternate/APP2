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
    current_cel = seq->tete;
    pile *pile_calculette = creer_pile();

    while (current_cel != NULL) { 
        printf("%c\n",current_cel->command);
        commande = current_cel->command;
        switch (commande) {
            case 'A':
                ret = avance();
                if (ret == VICTOIRE){
                  detruire_pile(pile_calculette);
                  return VICTOIRE;
                }
                if (ret == RATE){
                  detruire_pile(pile_calculette);
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
                pose(depiler(pile_calculette));
                break;
            case 'M':
                empiler(pile_calculette,mesure(depiler(pile_calculette)));
                break;
            case '+':
                addition(pile_calculette);
                break;
            case '-':
                soustraction(pile_calculette);
                break;
            case '*':
                multiplication(pile_calculette);
                break;
            case '?':
                condition(pile_calculette,current_cel)
                
            default:
                printf("Caractère spécial: '%c'\n", commande);
                if (commande <= '9' && '0' <= commande){
                  empiler(pile_calculette,(commande-'0'));
                }
                afficher_pile(pile_calculette);
                break;
        }
        current_cel = current_cel->suivant;
        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        if (debug) stop();
    }
    
    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    detruire_pile(p);
    return CIBLERATEE;
}
